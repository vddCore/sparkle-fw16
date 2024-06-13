#include <pico/stdio.h>
#include <pico/stdio_usb.h>

#include "sparkle.h"

int main(void)
{
    stdio_init_all();
    stdio_usb_init();
    
    while (!stdio_usb_connected())
    {
        sleep_ms(1000);
    }

    sparkle_context_t* sparkle = sparkle_init();
    sparkle_main(sparkle);

    /*
     * Execution should never reach this point.
     */
    sparkle_exit(sparkle);
    return 0;
}
