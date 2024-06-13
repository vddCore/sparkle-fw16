#include <pico/stdio.h>

#include "sparkle.h"

extern bool stdio_usb_init(void);

int main(void)
{
    stdio_init_all();
    stdio_usb_init();
    
    sparkle_context_t* sparkle = sparkle_init();
    sparkle_main(sparkle);

    /*
     * Execution should never reach this point.
     */
    sparkle_exit(sparkle);
    return 0;
}
