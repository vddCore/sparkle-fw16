#ifndef USB_STDIO_H
#define USB_STDIO_H

#include <pico/stdio.h>

#define USB_STDIO_DEFAULT_CRLF PICO_STDIO_DEFAULT_CRLF
#define USB_STDIO_STDOUT_TIMEOUT_US 500000
#define USB_STDIO_CONNECTION_WITHOUT_DTR 0
#define USB_STDIO_DEVICE_SELF_POWERED 0
#define USB_STDIO_NO_BI 0
#define USB_STDIO_ITF 0

#ifdef __cplusplus
extern "C" {
#endif

extern stdio_driver_t stdio_usb;

bool usb_stdio_init(void);
bool usb_stdio_connected(void);
    
#ifdef __cplusplus
}
#endif

#endif // USB_STDIO_H
