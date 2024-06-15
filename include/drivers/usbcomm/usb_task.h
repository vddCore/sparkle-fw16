#ifndef USB_TASK_H
#define USB_TASK_H

#include <stdint.h>
#include <stdbool.h>

#define USB_TASK_INTERVAL_US 1000
#define USB_TASK_RX_CALLBACKS_MAX CFG_TUD_CDC + 1 // +1 because the reset interface exists.

typedef void (*usb_rx_callback_t)(void*);

typedef struct usb_rx_callback_info
{
    usb_rx_callback_t cb;
    void* parameter;
} usb_rx_callback_info_t;

bool usb_task_init(void);
bool usb_task_is_initialized(void);
bool usb_task_set_rx_callback(uint8_t itf, usb_rx_callback_t cb, void* parameter);

#endif //USB_TASK_H
