#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "debug/log.h"
#include "drivers/pins.h"
#include "drivers/is3741.h"
#include "drivers/led_matrix.h"

#include "sparkle/kernel.h"

static void kernel_i2c_init(kernel_context_t* sparkle)
{
    i2c_init(SPARKLE_I2C_INSTANCE, IS3741_I2C_FREQ);
}

static int32_t kernel_led_matrix_init(kernel_context_t* sparkle)
{
    int32_t result = (int32_t)is3741_init(IS3741_I2C_ADDR, &sparkle->is3741);

#if LED_MATRIX_USE_EVT_LUT
        if (!result) result = (int32_t)is3741_set_sws_config(sparkle->is3741, IS3741_SWS_SW1SW9);
#else
    if (!result) result = (int32_t)is3741_set_sws_config(sparkle->is3741, IS3741_SWS_SW1SW8);
#endif

    if (!result) result = is3741_set_led_scaling(sparkle->is3741, 50);
    if (!result) result = is3741_set_pwm_freq(sparkle->is3741, IS3741_PFS_29000HZ);
    
    if (!result)
    {
        led_matrix_set_controller(sparkle->is3741);
        led_matrix_clear();
    }

    pins_set_is3741_sdb(true);
    return result;
}

kernel_context_t* kernel_init(void)
{
    kernel_context_t* sparkle = (kernel_context_t*)calloc(1, sizeof(kernel_context_t));
    
    if (!sparkle)
    {
        while (true)
        {
            log_error("Unable to allocate memory for context structure.");;
            sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
        }
    }

    kernel_i2c_init(sparkle);

    if (kernel_led_matrix_init(sparkle) < 0)
    {
        free(sparkle);

        while (true)
        {
            log_error("Unable to initialize LED matrix.");
            sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
        }
    }

    return sparkle;
}

_Noreturn void kernel_panic(void)
{
    while (true)
    {
        log_error("PANIC!");
        sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
    }
}

void kernel_exit(kernel_context_t* sparkle)
{
    if (!sparkle)
    {
        log_warn("Attempt to destroy a NULL Sparkle context.");
        return;
    }

    free(sparkle);
}

_Noreturn void kernel_main(kernel_context_t* sparkle)
{    
    log_info("Entering main system loop.");
    
    while (true)
    {
    }
}
