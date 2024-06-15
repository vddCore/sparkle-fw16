#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_DEVICE)

#define CFG_TUD_CDC             (1)
#define CFG_TUD_CDC_RX_BUFSIZE  (256)
#define CFG_TUD_CDC_TX_BUFSIZE  (256)

#define CFG_TUD_VENDOR          (0)

#define CFG_TUD_HID             (1)
#define CFG_TUD_HID_EP_BUFSIZE  (307)

#endif //TUSB_CONFIG_H
