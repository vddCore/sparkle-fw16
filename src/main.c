#include <pico/stdio.h>

#include "pins.h"
#include "usbcomm/usb_task.h"
#include "usbcomm/usb_stdio.h"
#include "usbcomm/usb_control.h"

#include "sparkle.h"

int main(void)
{
    pins_init();
    usb_task_init();
    usb_stdio_init();
    usb_control_init();

    sparkle_context_t* sparkle = sparkle_init();
    sparkle_main(sparkle);

    /*
     * Execution should never reach this point.
     */
    sparkle_exit(sparkle);
    return 0;
}
