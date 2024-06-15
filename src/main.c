#include <pico/stdio.h>

#include "drivers/usbcomm/usb_task.h"
#include "drivers/usbcomm/usb_stdio.h"

#include "sparkle/kernel.h"

#include "drivers/pins.h"

int main(void)
{
    pins_init();
    
    usb_task_init();
    usb_stdio_init();

    kernel_context_t* sparkle = kernel_init();
    kernel_main(sparkle);

    /*
     * Execution should never reach this point.
     */
    kernel_exit(sparkle);
    return 0;
}
