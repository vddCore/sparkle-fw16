#include <tusb.h>

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED) && CFG_TUD_CDC
#include <pico/binary_info.h>
#include <pico/mutex.h>
#include <pico/time.h>
#include <pico/stdio/driver.h>

#include "drivers/usbcomm/usb_stdio.h"
#include "drivers/usbcomm/usb_task.h"

static mutex_t usb_stdio_mutex;

static void usb_stdio_push_data(const char* buffer, int length)
{
    if (!length) return;
    
    static uint64_t last_avail_time;

    if (!mutex_try_enter_block_until(&usb_stdio_mutex, make_timeout_time_ms(PICO_STDIO_DEADLOCK_TIMEOUT_MS)))
    {
        return;
    }
    
    if (usb_stdio_connected())
    {
        for (int32_t i = 0; i < length;)
        {
            int32_t remaining = length - i;
            int32_t available = (int32_t)tud_cdc_n_write_available(USB_STDIO_ITF);
            
            if (remaining > available)
                remaining = available;
            
            if (remaining > 0)
            {
                int32_t written = (int32_t)tud_cdc_n_write(
                    USB_STDIO_ITF,
                    buffer + i,
                    (uint32_t)remaining
                );
                
                tud_task();
                tud_cdc_n_write_flush(USB_STDIO_ITF);

                i += written;
                last_avail_time = time_us_64();
            }
            else
            {
                tud_task();
                tud_cdc_n_write_flush(USB_STDIO_ITF);
                
                if (!usb_stdio_connected()
                    || (!tud_cdc_n_write_available(USB_STDIO_ITF)
                        && time_us_64() > last_avail_time + USB_STDIO_STDOUT_TIMEOUT_US))
                {
                    break;
                }
            }
        }
    }
    else
    {
        last_avail_time = 0;
    }
    
    mutex_exit(&usb_stdio_mutex);
}

static int usb_stdio_pull_data(char* buffer, int length)
{
    int32_t rc = PICO_ERROR_NO_DATA;
    
    if (usb_stdio_connected() && tud_cdc_n_available(USB_STDIO_ITF))
    {
        if (!mutex_try_enter_block_until(&usb_stdio_mutex, make_timeout_time_ms(PICO_STDIO_DEADLOCK_TIMEOUT_MS)))
        {
            return PICO_ERROR_TIMEOUT;
        }
        
        if (usb_stdio_connected() && tud_cdc_n_available(USB_STDIO_ITF))
        {
            int32_t count = (int32_t)tud_cdc_n_read(
                USB_STDIO_ITF,
                buffer,
                (uint32_t)length
            );
            
            rc = count ? count : PICO_ERROR_NO_DATA;
        }
        else
        {
            tud_task();
        }
        
        mutex_exit(&usb_stdio_mutex);
    }
    return rc;
}

stdio_driver_t stdio_usb = {
    .out_chars = usb_stdio_push_data,
    .in_chars = usb_stdio_pull_data,
#if PICO_STDIO_ENABLE_CRLF_SUPPORT
    .crlf_enabled = USB_STDIO_DEFAULT_CRLF
#endif
};

bool usb_stdio_init(void)
{
    if (!usb_task_is_initialized())
    {
        return false;
    }

    mutex_init(&usb_stdio_mutex);
    stdio_set_driver_enabled(&stdio_usb, true);
    
    return true;
}

bool usb_stdio_connected(void)
{
    return tud_cdc_n_connected(USB_STDIO_ITF);
}

#else
#warning CDC is not enabled - USB standard IO will not be initialized.
bool stdio_usb_init(void) {
    return false;
}
#endif // CFG_TUD_ENABLED && CFG_TUD_CDC
