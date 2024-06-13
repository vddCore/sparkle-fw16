#include <tusb.h>

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED) && CFG_TUD_CDC
#include <pico/binary_info/code.h>

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

int32_t usb_control_read(uint8_t* buffer, size_t len)
{
    int32_t rc = PICO_ERROR_NO_DATA;

    if (usb_control_connected() && tud_cdc_n_available(USB_CONTROL_ITF))
    {
        int32_t count = (int32_t)tud_cdc_n_read(USB_CONTROL_ITF, buffer, len);
        rc = count ? count : PICO_ERROR_NO_DATA;
    }
    
    return rc;
}

int32_t usb_control_write(uint8_t* data, size_t len)
{
    if (len == 0) return 0;
    
    int32_t rc = PICO_ERROR_GENERIC;

    if (usb_control_connected())
    {
        int32_t written_bytes = 0;
        
        size_t bytes_left = len;
        uint32_t max_bytes_per_cycle = tud_cdc_n_write_available(USB_CONTROL_ITF);

        if (max_bytes_per_cycle)
        {
            while (bytes_left > 0)
            {
                if (max_bytes_per_cycle < bytes_left)
                {
                    written_bytes += tud_cdc_n_write(
                        USB_CONTROL_ITF,
                        data + (len - bytes_left),
                        max_bytes_per_cycle
                    );
                    
                    bytes_left -= max_bytes_per_cycle;
                }
                else
                {
                    written_bytes += tud_cdc_n_write(
                        USB_CONTROL_ITF,
                        data + (len - bytes_left),
                        bytes_left
                    );
                    
                    bytes_left = 0;
                }
            }

            tud_cdc_n_write_flush(USB_CONTROL_ITF);
            rc = written_bytes;
        }
    }
    
    return rc;
}

#else
#warning CDC is not enabled - USB control port will not be initialized.
bool stdio_usb_init(void) {
    return false;
}
#endif // CFG_TUD_ENABLED && CFG_TUD_CDC