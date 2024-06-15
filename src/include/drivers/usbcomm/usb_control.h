#ifndef USB_CONTROL_H
#define USB_CONTROL_H

#define USB_CONTROL_ITF 1

#define USB_CONTROL_WRITE_TIMEOUT_MS 1000
#define USB_CONTROL_WRITE_BUFFER_TIMEOUT_US 500000

#define USB_CONTROL_READ_BUFFER_TIMEOUT_US 2500

bool usb_control_init(void);
bool usb_control_connected(void);

int32_t usb_control_read(uint8_t* buffer, uint32_t length);
int32_t usb_control_write(uint8_t* data, uint32_t length);

#endif //USB_CONTROL_H
