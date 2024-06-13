﻿#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <pico/binary_info/code.h>
#include <hardware/i2c.h>

#include "sparkle.h"
#include "is3741.h"
#include "log.h"

static void sparkle_gpio_init(sparkle_context_t* context)
{
    gpio_set_function(SPARKLE_GPIO_SDB, GPIO_FUNC_SIO);
    gpio_set_outover(SPARKLE_GPIO_SDB, GPIO_OVERRIDE_HIGH);
    gpio_set_oeover(SPARKLE_GPIO_SDB, GPIO_OVERRIDE_HIGH);
    gpio_set_dir(SPARKLE_GPIO_SDB, true);
    gpio_put(SPARKLE_GPIO_SDB, true);

    gpio_set_function(SPARKLE_GPIO_SLEEP, GPIO_FUNC_SIO);
    gpio_set_dir(SPARKLE_GPIO_SLEEP, false);
    
    gpio_set_function(SPARKLE_GPIO_DIP1, GPIO_FUNC_SIO);
    gpio_set_dir(SPARKLE_GPIO_DIP1, false);
    gpio_pull_up(SPARKLE_GPIO_DIP1);
    
    gpio_set_function(SPARKLE_GPIO_INTB, GPIO_FUNC_SIO);
    gpio_set_dir(SPARKLE_GPIO_INTB, false);
}

static void sparkle_i2c_init(sparkle_context_t* context)
{
    context->i2c_baudrate = i2c_init(SPARKLE_I2C_INSTANCE, IS3741_I2C_FREQ);

    gpio_set_function(SPARKLE_GPIO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SPARKLE_GPIO_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(SPARKLE_GPIO_SDA);
    gpio_pull_up(SPARKLE_GPIO_SCL);
}

sparkle_context_t* sparkle_init(void)
{
    sparkle_context_t* context = (sparkle_context_t*)calloc(1, sizeof(sparkle_context_t));

    if (!context)
    {
        while (true)
        {
            log_error("Unable to allocate memory for context structure.");;
            sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
        }
    }

    sparkle_gpio_init(context);
    sparkle_i2c_init(context);
    
    if (is3741_init(IS3741_I2C_ADDR, &context->is3741) < 0)
    {
        free(context);

        while (true)
        {            
            log_error("Unable to initialize the IS3741 controller context.");
            sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
        }
    }

    sleep_ms(10);

#ifdef IS3741_USE_EVT_LUT
    is3741_set_sws_config(context->is3741, IS3741_SWS_SW1SW9);
#else
    is3741_set_sws_config(context->is3741, IS3741_SWS_SW1SW8);
#endif

    if (is3741_set_led_scaling(context->is3741, 255) < 0)
    {
        free(context);

        while (true)
        {            
            log_error("Unable to set LED scaling.");
            sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
        }
    }

    if (is3741_set_pwm_freq(context->is3741, IS3741_PFS_29000HZ) < 0)
    {
        free(context);

        while (true)
        {            
            log_error("Unable to set PWM frequency.");
            sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
        }
    }
    
    return context;
}

_Noreturn void sparkle_panic(void)
{
    while (true)
    {
        log_error("PANIC!");
        sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
    }
}

void sparkle_exit(sparkle_context_t* context)
{
    if (!context)
    {
        log_warn("Attempt to destroy a NULL context.");
        return;
    }

    free(context);
}

_Noreturn void sparkle_main(sparkle_context_t* context)
{
    log_info("Entering main system loop.");
    log_info("Hello, world! I2C baud rate: %d", context->i2c_baudrate);

    while (true)
    {
        sleep_us(1);
    }
}
