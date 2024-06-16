#include <tusb.h>
#include <debug/log.h>

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED) && CFG_TUD_HID

#include "drivers/pins.h"
#include "drivers/is3741.h"
#include "drivers/led_matrix.h"
#include "drivers/usbcomm/usb_control.h"
#include "drivers/usbcomm/usb_task.h"

static kernel_context_t* _kernel;

uint16_t tud_hid_get_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t* buffer,
    uint16_t reqlen)
{
    if (!usb_task_is_initialized())
        return 0;

    switch (report_id)
    {
        case REPORT_ID_GET_PROPERTIES:
        {
            buffer[0] = (uint8_t)pins_get_sleep();
            buffer[1] = (uint8_t)pins_get_dip1();
            buffer[2] = (uint8_t)pins_get_intb();
            buffer[3] = (uint8_t)(_kernel->state_flags & SPARKLE_STATEFLAG_WAKE_ON_COMMAND);
            buffer[4] = (uint8_t)((_kernel->state_flags & SPARKLE_STATEFLAG_SLEEP_STATE) >> 1);
            buffer[5] = (uint8_t)((_kernel->state_flags & SPARKLE_STATEFLAG_SLEEP_IGNORE) >> 3);

            is3741_read_id_register(_kernel->is3741, &buffer[6]);
            is3741_read_config_register(_kernel->is3741, &buffer[7]);
            is3741_get_global_current(_kernel->is3741, &buffer[8]);
            buffer[9] = (uint8_t)LED_MATRIX_WIDTH;
            buffer[10] = (uint8_t)LED_MATRIX_HEIGHT;
            // 11..31 reserved.
            break;
        }

        case REPORT_ID_GRID_CNTL:
        {
            led_matrix_get_bitmap(buffer);
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
    if (!usb_task_is_initialized())
        return;

    switch (report_id)
    {
        case REPORT_ID_BASIC_CMD:
            break;

        case REPORT_ID_GRID_CNTL:
            led_matrix_bitmap(buffer, LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT);
            break;
    }
}

void usb_control_init(kernel_context_t* kernel)
{
    _kernel = kernel;
}

#endif
