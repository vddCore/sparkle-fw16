#include <tusb.h>

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED) && CFG_TUD_CDC
#include <pico/binary_info.h>
#include <pico/stdio/driver.h>
#include <pico/mutex.h>

#include "usb_stdio.h"
#include "usb_task.h"

static mutex_t usb_stdio_mutex;

static void usb_stdio_out_chars(const char* buf, int length)
{
    static uint64_t last_avail_time;

    if (!mutex_try_enter_block_until(&usb_stdio_mutex, make_timeout_time_ms(PICO_STDIO_DEADLOCK_TIMEOUT_MS)))
    {
        return;
    }
    
    if (usb_stdio_connected())
    {
        for (int i = 0; i < length;)
        {
            int n = length - i;
            int avail = (int)tud_cdc_n_write_available(USB_STDIO_ITF);
            if (n > avail) n = avail;
            if (n)
            {
                int n2 = (int)tud_cdc_write(buf + i, (uint32_t)n);
                tud_task();
                tud_cdc_n_write_flush(USB_STDIO_ITF);
                i += n2;
                last_avail_time = time_us_64();
            }
            else
            {
                tud_task();
                tud_cdc_n_write_flush(USB_STDIO_ITF);
                if (!usb_stdio_connected() ||
                    (!tud_cdc_n_write_available(USB_STDIO_ITF) && time_us_64() > last_avail_time + USB_STDIO_STDOUT_TIMEOUT_US))
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

int usb_stdio_in_chars(char* buf, int length)
{
    int rc = PICO_ERROR_NO_DATA;
    if (usb_stdio_connected() && tud_cdc_n_available(USB_STDIO_ITF))
    {
        if (!mutex_try_enter_block_until(&usb_stdio_mutex, make_timeout_time_ms(PICO_STDIO_DEADLOCK_TIMEOUT_MS)))
        {
            return PICO_ERROR_NO_DATA;
        }
        
        if (usb_stdio_connected() && tud_cdc_n_available(USB_STDIO_ITF))
        {
            int count = (int)tud_cdc_n_read(USB_STDIO_ITF, buf, (uint32_t)length);
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
    .out_chars = usb_stdio_out_chars,
    .in_chars = usb_stdio_in_chars,
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
#if USB_STDIO_CONNECTION_WITHOUT_DTR
    return tud_ready();
#else
    return tud_cdc_n_connected(USB_STDIO_ITF);
#endif
}

#else
#warning CDC is not enabled - USB standard IO will not be initialized.
bool stdio_usb_init(void) {
    return false;
}
#endif // CFG_TUD_ENABLED && CFG_TUD_CDC
