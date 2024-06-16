#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_DEVICE)

#define CFG_TUD_CDC              (1)
#define CFG_TUD_CDC_RX_BUFSIZE   (256)
#define CFG_TUD_CDC_TX_BUFSIZE   (256)

#define CFG_TUD_VENDOR           (0)

#define CFG_TUD_HID              (1)
#define CFG_TUD_HID_EP_BUFSIZE   (384)

#define REPORT_ID_DEVICE_INFO        (0x01)
#define REPORT_ID_BASIC_CMD          (0x02)
#define REPORT_ID_GRID_PWM_CNTL      (0x03)
#define REPORT_ID_GRID_DC_SCALE_CNTL (0x04)

#endif //TUSB_CONFIG_H
