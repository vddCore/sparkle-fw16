#include <pico/stdio.h>

#include "sparkle.h"
#include "usbcomm/usb_stdio.h"

int main(void)
{
    stdio_init_all();
    usb_stdio_init();
    
    sparkle_context_t* sparkle = sparkle_init();
    sparkle_main(sparkle);

    /*
     * Execution should never reach this point.
     */
    sparkle_exit(sparkle);
    return 0;
}
