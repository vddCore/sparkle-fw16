#ifndef USB_CONTROL_H
#define USB_CONTROL_H

#define USB_CONTROL_ITF 1
#define USB_CONTROL_NO_BI 0

bool usb_control_init(void);
bool usb_control_connected(void);
int32_t usb_control_read(uint8_t* buffer, size_t len);
int32_t usb_control_write(uint8_t* data, size_t len);

#endif //USB_CONTROL_H
