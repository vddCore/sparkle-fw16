#include <tusb.h>

#include "../log.h"

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED) && CFG_TUD_HID
#include "usb_control.h"
#include "usb_task.h"

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    if (!usb_task_is_initialized())
        return 0;

    log_info("Got get-report request: %d, reqlen %d", report_id, reqlen);

    memset(buffer, 0xAA, reqlen);
    return reqlen;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    if (!usb_task_is_initialized())
        return;

    log_info("Got set-report request: %d, reqlen %d", report_id, bufsize);
}
#endif