#include <tusb.h>
#include <pico/unique_id.h>

#include "drivers/usbcomm/usb_reset_interface.h"

#define PID_MAP(itf, n) ((CFG_TUD_##itf) << (n))

//==============================================================//
//  [DEVICE DESCRIPTION]                                        //
//==============================================================//
#define USBD_VID (0x32AC) // Framework Computer Inc
#define USBD_PID (0x0020) // LED Matrix Input Module

#define USBD_DESC_STR_MAX (96)
#define USBD_MANUFACTURER "Framework Computer Inc"
#define USBD_PRODUCT "LED Matrix Input Module | SPARKLE"
#define USBD_CDC_PORT_NAME "LED Matrix CDC Debug Port"
#define USBD_HID_PORT_NAME "LED Matrix HID Control Port"
#define USBD_RST_PORT_NAME "LED Matrix Reset Port"

enum
{
    USBD_STR_LANGUAGE      = 0x00,
    USBD_STR_MANUFACTURER  = 0x01,
    USBD_STR_PRODUCT       = 0x02,
    USBD_STR_SERIAL_NUMBER = 0x03,
    USBD_STR_CDC_PORT_NAME = 0x04,
    USBD_STR_HID_PORT_NAME = 0x05,
    USBD_STR_RST_PORT_NAME = 0x06
};

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
    .iManufacturer = USBD_STR_MANUFACTURER,
    .iProduct = USBD_STR_PRODUCT,
    .iSerialNumber = USBD_STR_SERIAL_NUMBER,
    .bNumConfigurations = 1,
};

//==============================================================//
//  [ENDPOINT DEFINITIONS]                                      //
//==============================================================//

// ---/ SERIAL CDC ENDPOINT /---------------------------------- //
//
#define USBD_CDC_CMD_EP         (0x81)
#define USBD_CDC_DATA_EP        (0x82)
#define USBD_CDC_CMD_SIZE         (64)
#define USBD_CDC_DATA_SIZE        (64)

// ---/ HID ENDPOINT /------------------------------------------// 
#define USBD_HID_EP             (0x83)
#define USBD_HID_BUFSIZE          (16)
#define USBD_HID_POLL_INTERVAL    (1)

#define USBD_LED_MATRIX_FEATURE_DESCRIPTOR                         \
    HID_USAGE_PAGE_N(HID_USAGE_PAGE_VENDOR, 2),                    \
    HID_USAGE(0x01),                                               \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),                    \
        HID_REPORT_ID(REPORT_ID_DEVICE_INFO)                       \
        HID_USAGE(0x02),                                           \
        HID_LOGICAL_MIN(0x00),                                     \
        HID_LOGICAL_MAX_N(0xFF, 2),                                \
        HID_REPORT_SIZE(8),                                        \
        HID_REPORT_COUNT(32),                                      \
        HID_FEATURE(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),       \
                                                                   \
        HID_REPORT_ID(REPORT_ID_BASIC_CMD)                         \
        HID_USAGE(0x03),                                           \
        HID_REPORT_SIZE(8),                                        \
        HID_REPORT_COUNT(16),                                      \
        HID_FEATURE(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),       \
                                                                   \
        HID_REPORT_ID(REPORT_ID_GRID_PWM_CNTL)                     \
        HID_USAGE(0x04),                                           \
        HID_REPORT_SIZE(8),                                        \
        HID_REPORT_COUNT_N(306, 2),                                \
        HID_FEATURE(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),       \
                                                                   \
        HID_REPORT_ID(REPORT_ID_GRID_DC_SCALE_CNTL)                \
        HID_USAGE(0x05),                                           \
        HID_REPORT_SIZE(8),                                        \
        HID_REPORT_COUNT_N(306, 2),                                \
        HID_FEATURE(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),       \
    HID_COLLECTION_END

static const uint8_t desc_hid_report[] = { USBD_LED_MATRIX_FEATURE_DESCRIPTOR };

//==============================================================//
//  [ENDPOINT DEFINITIONS]                                      //
//==============================================================//

#define USBD_DESC_LEN ((TUD_CONFIG_DESC_LEN) +            \
                       (TUD_CDC_DESC_LEN * CFG_TUD_CDC) + \
                       (TUD_HID_DESC_LEN * CFG_TUD_HID) + \
                       (TUD_RPI_RESET_DESC_LEN))

enum
{
    ITF_NUM_CDC,
    ITF_NUM_CDC_DATA,
    ITF_NUM_HID,
    ITF_NUM_RST,
    ITF_NUM_TOTAL
};

#define USBD_MAX_POWER_MA (250)

static const uint8_t usbd_desc_cfg[USBD_DESC_LEN] = {
    TUD_CONFIG_DESCRIPTOR(
        1,
        ITF_NUM_TOTAL,
        USBD_STR_LANGUAGE,
        USBD_DESC_LEN,
        TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP,
        USBD_MAX_POWER_MA
    ),

    TUD_CDC_DESCRIPTOR(
        ITF_NUM_CDC,
        USBD_STR_CDC_PORT_NAME,
        USBD_CDC_CMD_EP, USBD_CDC_CMD_SIZE,
        USBD_CDC_DATA_EP & 0x7F,
        USBD_CDC_DATA_EP, USBD_CDC_DATA_SIZE
    ),

    TUD_HID_DESCRIPTOR(
        ITF_NUM_HID,
        USBD_STR_HID_PORT_NAME,
        HID_ITF_PROTOCOL_NONE,
        sizeof(desc_hid_report),
        USBD_HID_EP,
        USBD_HID_BUFSIZE,
        USBD_HID_POLL_INTERVAL
    ),

    TUD_RPI_RESET_DESCRIPTOR(ITF_NUM_RST, USBD_STR_RST_PORT_NAME)
};

static char usbd_serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];

static const char* const usbd_desc_str[] = {
    [USBD_STR_MANUFACTURER] = USBD_MANUFACTURER,
    [USBD_STR_PRODUCT] = USBD_PRODUCT,
    [USBD_STR_SERIAL_NUMBER] = usbd_serial_str,
    [USBD_STR_CDC_PORT_NAME] = USBD_CDC_PORT_NAME,
    [USBD_STR_HID_PORT_NAME] = USBD_HID_PORT_NAME,
    [USBD_STR_RST_PORT_NAME] = USBD_RST_PORT_NAME
};

const uint8_t* tud_descriptor_device_cb(void) { return (const uint8_t*)&usbd_desc_device; }
const uint8_t* tud_descriptor_configuration_cb(__unused uint8_t index) { return usbd_desc_cfg; }
const uint8_t* tud_hid_descriptor_report_cb(uint8_t instance) { return (const uint8_t*)desc_hid_report; }

const uint16_t* tud_descriptor_string_cb(uint8_t index, __unused uint16_t langid)
{
    static uint16_t desc_str[USBD_DESC_STR_MAX] = { 0 };

    if (!usbd_serial_str[0])
    {
        pico_get_unique_board_id_string(usbd_serial_str, sizeof(usbd_serial_str));
    }

    uint8_t len;
    if (index == USBD_STR_LANGUAGE)
    {
        desc_str[1] = 0x0409; // supported language is English
        len = 1;
    }
    else
    {
        if (index >= sizeof(usbd_desc_str) / sizeof(usbd_desc_str[0]))
        {
            return NULL;
        }

        const char* str = usbd_desc_str[index];
        for (len = 0; len < USBD_DESC_STR_MAX - 1 && str[len]; ++len)
        {
            desc_str[1 + len] = str[len];
        }
    }

    // first byte is length (including header), second byte is string type
    desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * len + 2));

    return desc_str;
}
