#include <tusb.h>
#include <pico/unique_id.h>

#include "usb_reset_interface.h"

#define USBD_VID (0x32AC) // Framework Computer Inc
#define USBD_PID (0x0020) // LED Matrix Input Module
#define USBD_MANUFACTURER "Framework Computer Inc"
#define USBD_PRODUCT "LED Matrix Input Module | SPARKLE"

#define TUD_RPI_RESET_DESC_LEN  9

#define USBD_DESC_LEN (TUD_CONFIG_DESC_LEN + (TUD_CDC_DESC_LEN * CFG_TUD_CDC) + TUD_RPI_RESET_DESC_LEN)
#define USBD_CONFIGURATION_DESCRIPTOR_ATTRIBUTE (0)
#define USBD_MAX_POWER_MA (250)

#define USBD_ITF_CDC_0     (0)
#define USBD_ITF_CDC_1     (2)
#define USBD_ITF_RPI_RESET (4)
#define USBD_ITF_MAX       (5)

#define USBD_CDC_0_EP_CMD (0x81)
#define USBD_CDC_0_EP_OUT (0x02)
#define USBD_CDC_0_EP_IN  (0x82)

#define USBD_CDC_1_EP_CMD (0x83)
#define USBD_CDC_1_EP_OUT (0x03)
#define USBD_CDC_1_EP_IN  (0x84)

#define USBD_CDC_CMD_MAX_SIZE (8)
#define USBD_CDC_IN_OUT_MAX_SIZE (64)

#define USBD_STR_0 (0x00)
#define USBD_STR_MANUF (0x01)
#define USBD_STR_PRODUCT (0x02)
#define USBD_STR_SERIAL (0x03)
#define USBD_STR_CDC_0 (0x04)
#define USBD_STR_CDC_1 (0x05)
#define USBD_STR_RPI_RESET (0x06)

static const tusb_desc_device_t usbd_desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = USBD_VID,
    .idProduct = USBD_PID,
    .bcdDevice = 0x0100,
    .iManufacturer = USBD_STR_MANUF,
    .iProduct = USBD_STR_PRODUCT,
    .iSerialNumber = USBD_STR_SERIAL,
    .bNumConfigurations = 1,
};

#define TUD_RPI_RESET_DESCRIPTOR(_itfnum, _stridx) \
  9, TUSB_DESC_INTERFACE, _itfnum, 0, 0, TUSB_CLASS_VENDOR_SPECIFIC, USB_RESET_INTERFACE_SUBCLASS, USB_RESET_INTERFACE_PROTOCOL, _stridx,

static const uint8_t usbd_desc_cfg[USBD_DESC_LEN] = {
    TUD_CONFIG_DESCRIPTOR(1, USBD_ITF_MAX, USBD_STR_0, USBD_DESC_LEN,
        USBD_CONFIGURATION_DESCRIPTOR_ATTRIBUTE, USBD_MAX_POWER_MA),

    TUD_CDC_DESCRIPTOR(USBD_ITF_CDC_0, USBD_STR_CDC_0, USBD_CDC_0_EP_CMD,
        USBD_CDC_CMD_MAX_SIZE, USBD_CDC_0_EP_OUT, USBD_CDC_0_EP_IN, USBD_CDC_IN_OUT_MAX_SIZE),

    TUD_CDC_DESCRIPTOR(USBD_ITF_CDC_1, USBD_STR_CDC_1, USBD_CDC_1_EP_CMD,
        USBD_CDC_CMD_MAX_SIZE, USBD_CDC_1_EP_OUT, USBD_CDC_1_EP_IN, USBD_CDC_IN_OUT_MAX_SIZE),
    
    TUD_RPI_RESET_DESCRIPTOR(USBD_ITF_RPI_RESET, USBD_STR_RPI_RESET)
};

static char usbd_serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];

static const char *const usbd_desc_str[] = {
    [USBD_STR_MANUF] = USBD_MANUFACTURER,
    [USBD_STR_PRODUCT] = USBD_PRODUCT,
    [USBD_STR_SERIAL] = usbd_serial_str,
    [USBD_STR_CDC_0] = "LED Matrix CDC Debug Port",
    [USBD_STR_CDC_1] = "LED Matrix CDC Control Port",
    [USBD_STR_RPI_RESET] = "LED Matrix Reset Port"
};

const uint8_t *tud_descriptor_device_cb(void) {
    return (const uint8_t *)&usbd_desc_device;
}

const uint8_t *tud_descriptor_configuration_cb(__unused uint8_t index) {
    return usbd_desc_cfg;
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, __unused uint16_t langid) {
#ifndef USBD_DESC_STR_MAX
#define USBD_DESC_STR_MAX (64)
#elif USBD_DESC_STR_MAX > 127
#error USBD_DESC_STR_MAX too high (max is 127).
#elif USBD_DESC_STR_MAX < 17
#error USBD_DESC_STR_MAX too low (min is 17).
#endif
    static uint16_t desc_str[USBD_DESC_STR_MAX];

    // Assign the SN using the unique flash id
    if (!usbd_serial_str[0]) {
        pico_get_unique_board_id_string(usbd_serial_str, sizeof(usbd_serial_str));
    }

    uint8_t len;
    if (index == 0) {
        desc_str[1] = 0x0409; // supported language is English
        len = 1;
    } else {
        if (index >= sizeof(usbd_desc_str) / sizeof(usbd_desc_str[0])) {
            return NULL;
        }
        const char *str = usbd_desc_str[index];
        for (len = 0; len < USBD_DESC_STR_MAX - 1 && str[len]; ++len) {
            desc_str[1 + len] = str[len];
        }
    }

    // first byte is length (including header), second byte is string type
    desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * len + 2));

    return desc_str;
}