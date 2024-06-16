#include <drivers/usbcomm/usb_control.h>

#include "drivers/usbcomm/usb_task.h"
#include "drivers/usbcomm/usb_stdio.h"
#include "drivers/pins.h"

#include "firmware/kernel.h"

int main(void)
{
    pins_init();
    
    // usb_task_init();
    usb_stdio_init();

    kernel_context_t* kernel = kernel_init();
    usb_control_init(kernel);
    
    kernel_main(kernel);

    /*
     * Execution should never reach this point.
     */
    kernel_exit(kernel);
    return 0;
}
