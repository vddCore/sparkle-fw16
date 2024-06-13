#include <pico/stdio.h>

#include "sparkle.h"

#include "usbcomm/usb_task.h"
#include "usbcomm/usb_stdio.h"
#include "usbcomm/usb_control.h"

int main(void)
{
    usb_task_init();
    usb_stdio_init();
    usb_control_init();

    while (!usb_stdio_connected())
    {
        sleep_ms(500);
    }
    
    sparkle_context_t* sparkle = sparkle_init();
    sparkle_main(sparkle);

    /*
     * Execution should never reach this point.
     */
    sparkle_exit(sparkle);
    return 0;
}
