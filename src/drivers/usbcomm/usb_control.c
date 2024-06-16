#include <tusb.h>

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED) && CFG_TUD_HID

#include <debug/log.h>
#include <hardware/watchdog.h>
#include <pico/bootrom.h>

#include "drivers/pins.h"
#include "drivers/is3741.h"
#include "drivers/led_matrix.h"
#include "protocol/glitter.h"
#include "firmware/services/srv_timeout.h"
#include "firmware/services/srv_sleep.h"

#include "drivers/usbcomm/usb_control.h"

static kernel_context_t* _kernel = NULL;

static glitter_device_info_t _devinfo = {
    /**
     * These won't magically change during
     * device operation so might as well
     * initialize them once and never touch
     * them again.
     **/
    .display_width = LED_MATRIX_WIDTH,
    .display_height = LED_MATRIX_HEIGHT
};

static void handle_basic_command(const uint8_t* buffer, uint16_t uint16)
{
    switch ((glitter_basic_command_t)buffer[0])
    {
        case GLITTER_CMD_REBOOT:
        {
            glitter_cmd_reboot_target_t target = (glitter_cmd_reboot_target_t)buffer[1];

            if (target == GLITTER_CMD_REBOOT_BOOTLOADER)
                reset_usb_boot(0, 0);

            if (target != GLITTER_CMD_REBOOT_FIRMWARE)
                log_warn("Unknown reboot target 0x%02X.", target);

           /**
            * Treat unknown reboot targets
            * as if they're firmware reboots.
            **/
            watchdog_reboot(0, 0, 1);
            break;
        }

        case GLITTER_CMD_SLEEP:
        {
            glitter_cmd_sleep_directive_t directive = (glitter_cmd_sleep_directive_t)buffer[1];

            if (directive == GLITTER_CMD_SLEEP_EXIT)
            {
                srv_sleep_exit(_kernel);
            }
            else
            {
                if (directive != GLITTER_CMD_SLEEP_ENTER)
                    log_warn("Unexpected sleep directive 0x%02X.", directive);

                /**
                 * Treat unknown sleep directives
                 * as if they're go-to-sleep requests.
                 **/
                srv_sleep_enter(_kernel, KERNEL_SLEEPSTATE_COMMAND);
            }

            break;
        }
        
        case GLITTER_CMD_SET_WAKE_ON_COMMAND:
        {
            if (buffer[1])
            {
                _kernel->state_flags |= KERNEL_STATEFLAG_WAKE_ON_COMMAND_MASK;    
            }
            else
            {
                _kernel->state_flags &= ~(KERNEL_STATEFLAG_WAKE_ON_COMMAND_MASK);    
            }
            break;
        }

        case GLITTER_CMD_SET_SLEEP_TIMEOUT:
        {
            int32_t timeout = (int32_t)(
                *(buffer + 1) << 24
                | *(buffer + 2) << 16
                | *(buffer + 3) << 8
                | *(buffer + 4)
            );
                
            log_info("Setting timeout to %dms", timeout);
            
            _kernel->sleep_timeout_ms = timeout;

            if (!srv_sleep_is_sleeping(_kernel))
                srv_timeout_refresh();
            
            if (_kernel->sleep_timeout_ms > 0)
            {
                srv_timeout_start(_kernel);
            }
            else
            {
                srv_timeout_stop(_kernel);
            }
            
            break;
        }

        case GLITTER_CMD_SET_GLOBAL_BRIGHTNESS:
        {
            srv_sleep_wake_on_command(_kernel);
            
            if (!srv_sleep_is_sleeping(_kernel))
                srv_timeout_refresh();
            
            is3741_set_global_current(_kernel->is3741, buffer[1]);
            break;
        }

        case GLITTER_CMD_DRAW_PIXEL:
        {
            srv_sleep_wake_on_command(_kernel);
            
            if (!srv_sleep_is_sleeping(_kernel))
                srv_timeout_refresh();
            
            led_matrix_set_pixel(
                buffer[1],
                buffer[2],
                buffer[3]
            );
            break;
        }

        case GLITTER_CMD_DRAW_LINE:
        {
            srv_sleep_wake_on_command(_kernel);
            
            if (!srv_sleep_is_sleeping(_kernel))
                srv_timeout_refresh();
            
            led_matrix_line(
                buffer[1], buffer[2],
                buffer[3], buffer[4],
                buffer[5]
            );
            break;
        }

        default:
        {
            log_warn("Unknown command %02X", buffer[0]);
            break;
        }
    }
}

uint16_t tud_hid_get_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t* buffer,
    uint16_t reqlen)
{
    if (!_kernel)
    {
        log_error("Kernel context is NULL.");
        return 0;
    }

    switch (report_id)
    {
        case REPORT_ID_DEVICE_INFO:
        {
            _devinfo.sleep_pin = (uint8_t)pins_get_sleep();
            _devinfo.dip1_pin = (uint8_t)pins_get_dip1();
            _devinfo.intb_pin = (uint8_t)pins_get_intb();
            _devinfo.state_flags = _kernel->state_flags;

            is3741_read_id_register(_kernel->is3741, &_devinfo.id_reg);
            is3741_read_config_register(_kernel->is3741, &_devinfo.config_reg);
            is3741_get_global_current(_kernel->is3741, &_devinfo.global_brightness);

            memcpy(&_devinfo, buffer, sizeof(_devinfo));
            break;
        }

        case REPORT_ID_GRID_PWM_CNTL:
        {
            led_matrix_get_pwm_bitmap(buffer);
            break;
        }

        case REPORT_ID_GRID_DC_SCALE_CNTL:
        {
            led_matrix_get_dc_scale_bitmap(buffer);
            break;
        }
    }

    return reqlen;
}

void tud_hid_set_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t const* buffer,
    uint16_t bufsize)
{
    if (!_kernel)
    {
        log_error("Kernel context is NULL.");
        return;
    }

    switch (report_id)
    {
        case REPORT_ID_BASIC_CMD:
        {
            handle_basic_command(buffer, bufsize);
            break;
        }
        
        case REPORT_ID_GRID_PWM_CNTL:
        {                
            srv_sleep_wake_on_command(_kernel);
            
            if (!srv_sleep_is_sleeping(_kernel))
                srv_timeout_refresh();
            
            led_matrix_set_pwm_bitmap(buffer, LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT);
            break;
        }

        case REPORT_ID_GRID_DC_SCALE_CNTL:
        {
            led_matrix_set_dc_scale_bitmap(buffer, LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT);
            break;
        }
    }
}

void tud_resume_cb(void)
{
    srv_sleep_exit(_kernel);
}

void tud_suspend_cb(bool remote_wakeup_en)
{
    srv_sleep_enter(_kernel, KERNEL_SLEEPSTATE_USB_SUSPEND);
}

void usb_control_init(kernel_context_t* kernel)
{
    _kernel = kernel;
}
#endif
