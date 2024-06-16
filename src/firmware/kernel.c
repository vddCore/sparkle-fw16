#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <pico/stdlib.h>

#include "debug/log.h"
#include "drivers/pins.h"
#include "drivers/is3741.h"
#include "drivers/led_matrix.h"

#include "firmware/kernel.h"

static int32_t kernel_led_matrix_init(kernel_context_t* kernel)
{
    int32_t result = (int32_t)is3741_init(IS3741_I2C_ADDR, &kernel->is3741);

#if LED_MATRIX_USE_EVT_LUT
        if (!result) result = (int32_t)is3741_set_sws_config(kernel->is3741, IS3741_SWS_SW1SW9);
#else
    if (!result) result = (int32_t)is3741_set_sws_config(kernel->is3741, IS3741_SWS_SW1SW8);
#endif

    if (!result) result = is3741_set_led_dc_scale_global(kernel->is3741, IS3741_DC_SCALE_DEFAULT);
    if (!result) result = is3741_set_pwm_freq(kernel->is3741, IS3741_PFS_29000HZ);
    
    if (!result)
    {
        led_matrix_set_controller(kernel->is3741);
        led_matrix_clear();
    }

    pins_set_is3741_sdb(true);
    return result;
}

kernel_context_t* kernel_init(void)
{
    kernel_context_t* kernel = (kernel_context_t*)calloc(1, sizeof(kernel_context_t));
    
    if (!kernel)
    {
        kernel_panic("Unable to allocate memory for context structure.");
    }

    if (kernel_led_matrix_init(kernel) < 0)
    {
        free(kernel);
        kernel_panic("Unable to initialize LED matrix.");
    }

    return kernel;
}

void kernel_exit(kernel_context_t* kernel)
{
    if (!kernel)
    {
        log_warn("Attempt to destroy a NULL kernel context.");
        return;
    }

    free(kernel);
}

_Noreturn void kernel_panic(const char* why)
{
    while (true)
    {
        log_error("PANIC! %s", why);
        sleep_ms(KERNEL_PANIC_INTERVAL_MS);
    }
}

_Noreturn void kernel_main(kernel_context_t* kernel)
{    
    log_info("Entering main system loop.");
    
    while (true)
    {
    }
}
