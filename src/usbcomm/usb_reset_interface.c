#include <tusb.h>
#include <pico/bootrom.h>
#include <hardware/watchdog.h>
#include <device/usbd_pvt.h>

#include "usb_stdio.h"
#include "usb_reset_interface.h"

static uint8_t itf_num;

static void resetd_init(void)
{
}

static void resetd_reset(uint8_t __unused rhport)
{
    itf_num = 0;
}

static uint16_t resetd_open(uint8_t __unused rhport, tusb_desc_interface_t const* itf_desc, uint16_t max_len)
{
    TU_VERIFY(TUSB_CLASS_VENDOR_SPECIFIC == itf_desc->bInterfaceClass &&
              USB_RESET_INTERFACE_SUBCLASS == itf_desc->bInterfaceSubClass &&
              USB_RESET_INTERFACE_PROTOCOL == itf_desc->bInterfaceProtocol, 0);

    uint16_t const drv_len = sizeof(tusb_desc_interface_t);
    TU_VERIFY(max_len >= drv_len, 0);

    itf_num = itf_desc->bInterfaceNumber;
    return drv_len;
}

static bool resetd_control_xfer_cb(uint8_t __unused rhport, uint8_t stage, tusb_control_request_t const* request)
{
    if (stage != CONTROL_STAGE_SETUP) return true;

    if (request->wIndex == itf_num)
    {
        if (request->bRequest == USB_RESET_REQUEST_BOOTSEL)
        {
            uint gpio_mask = 0u;

            if (request->wValue & 0x100)
            {
                gpio_mask = 1u << (request->wValue >> 9u);
            }

            reset_usb_boot(
                gpio_mask,
                (request->wValue & 0x7f) | USB_RESET_BOOTSEL_INTERFACE_DISABLE_MASK
            );
        }

        if (request->bRequest == USB_RESET_REQUEST_FLASH)
        {
            watchdog_reboot(0, 0, USB_RESET_TO_FLASH_DELAY_MS);
            return true;
        }
    }
    return false;
}

static bool resetd_xfer_cb(uint8_t __unused rhport, uint8_t __unused ep_addr, xfer_result_t __unused result, uint32_t __unused xferred_bytes)
{
    return true;
}

static usbd_class_driver_t const _resetd_driver =
{
#if CFG_TUSB_DEBUG >= 2
    .name = "RESET",
#endif
    .init = resetd_init,
    .reset = resetd_reset,
    .open = resetd_open,
    .control_xfer_cb = resetd_control_xfer_cb,
    .xfer_cb = resetd_xfer_cb,
    .sof = NULL
};

usbd_class_driver_t const* usbd_app_driver_get_cb(uint8_t* driver_count)
{
    *driver_count = 1;
    return &_resetd_driver;
}

void tud_cdc_line_coding_cb(__unused uint8_t itf, cdc_line_coding_t const* p_line_coding)
{
    if (p_line_coding->bit_rate == USB_RESET_MAGIC_BAUD_RATE)
    {
        const uint gpio_mask = 0u;
        reset_usb_boot(gpio_mask, USB_RESET_BOOTSEL_INTERFACE_DISABLE_MASK);
    }
}