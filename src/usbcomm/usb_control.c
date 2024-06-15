#include <tusb.h>

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED) && CFG_TUD_CDC
#include <pico/time.h>

#include "usb_control.h"
#include "usb_task.h"

bool usb_control_init(void)
{
    if (!usb_task_is_initialized())
    {
        return false;
    }

    return true;
}

bool usb_control_connected(void)
{
    return tud_cdc_n_connected(USB_CONTROL_ITF);
}

int32_t usb_control_read(uint8_t* buffer, uint32_t length)
{
    if (!length) return 0;

    static uint64_t last_write_avail_time;
    int32_t total_read = 0;
    
    if (usb_control_connected())
    {
        for (int32_t i = 0; i < length;)
        {
            int32_t remaining = length - i;
            int32_t available = (int32_t)tud_cdc_n_available(USB_CONTROL_ITF);

            if (available)
            {
                int32_t read_count = (int32_t)tud_cdc_n_read(
                    USB_CONTROL_ITF,
                    buffer + i,
                    (uint32_t)remaining
                );

                i += read_count;
                total_read += read_count;
                last_write_avail_time = time_us_64();
            }
            else
            {
                if (time_us_64() > last_write_avail_time + USB_CONTROL_READ_BUFFER_TIMEOUT_US)
                    break;
            }
        }
    }
    else
    {
        last_write_avail_time = 0;
    }

    return total_read;
}

int32_t usb_control_write(uint8_t* data, uint32_t length)
{
    if (!length) return 0;
    
    static uint64_t last_write_avail_time;

    uint32_t total_written = 0;
    int32_t rc = PICO_ERROR_GENERIC;
    
    if (usb_control_connected())
    {
        for (int32_t i = 0; i < length;)
        {
            int32_t remaining = length - i;
            int32_t available = (int32_t)tud_cdc_n_write_available(USB_CONTROL_ITF);

            if (remaining > available)
                remaining = available;

            if (remaining > 0)
            {
                int32_t written = (int32_t)tud_cdc_n_write(
                    USB_CONTROL_ITF,
                    data + i,
                    (uint32_t)remaining
                );

                tud_task();
                tud_cdc_n_write_flush(USB_CONTROL_ITF);
                
                i += written;
                total_written += written;
                last_write_avail_time = time_us_64();
            }
            else
            {
                tud_task();
                tud_cdc_n_write_flush(USB_CONTROL_ITF);

                if (!usb_control_connected()
                    || (!tud_cdc_n_write_available(USB_CONTROL_ITF)
                        && time_us_64() > last_write_avail_time + USB_CONTROL_WRITE_BUFFER_TIMEOUT_US))
                {
                    rc = PICO_ERROR_TIMEOUT;
                    break;
                }
            }

            rc = total_written;
        }
    }
    else
    {
        last_write_avail_time = 0;
    }

    return rc;
}
#else
#warning CDC is not enabled - USB control port will not be initialized.
bool stdio_usb_init(void) {
    return false;
}
#endif // CFG_TUD_ENABLED && CFG_TUD_CDC