#include <pico/stdio.h>
#include <pico/stdio_usb.h>

#include "sparkle.h"

int main(void)
{
    stdio_init_all();
    while (!stdio_usb_connected())
    {
        sleep_ms(1000);
    }

    sparkle_context_t* context = sparkle_init();
    sparkle_main(context);

    /*
     * Execution should never reach this point.
     */
    sparkle_exit(context);
    return 0;
}
