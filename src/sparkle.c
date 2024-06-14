#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "sparkle.h"
#include "log.h"
#include "is3741.h"
#include "led_matrix.h"
#include "usbcomm/usb_control.h"

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

static void sparkle_i2c_init(sparkle_context_t* sparkle)
{
    sparkle->i2c_baudrate = i2c_init(SPARKLE_I2C_INSTANCE, IS3741_I2C_FREQ);

    gpio_set_function(SPARKLE_GPIO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SPARKLE_GPIO_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(SPARKLE_GPIO_SDA);
    gpio_pull_up(SPARKLE_GPIO_SCL);
}

static int32_t sparkle_led_matrix_init(sparkle_context_t* sparkle)
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
    
    return result;
}

sparkle_context_t* sparkle_init(void)
{
    sparkle_context_t* sparkle = (sparkle_context_t*)calloc(1, sizeof(sparkle_context_t));

    if (!sparkle)
    {
        while (true)
        {
            log_error("Unable to allocate memory for context structure.");;
            sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
        }
    }

    sparkle_gpio_init(sparkle);
    sparkle_i2c_init(sparkle);
    
    if (sparkle_led_matrix_init(sparkle) < 0)
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

_Noreturn void sparkle_panic(void)
{
    while (true)
    {
        log_error("PANIC!");
        sleep_ms(SPARKLE_PANIC_SLEEP_INTERVAL_MS);
    }
}

void sparkle_exit(sparkle_context_t* sparkle)
{
    if (!sparkle)
    {
        log_warn("Attempt to destroy a NULL Sparkle context.");
        return;
    }

    free(sparkle);
}

_Noreturn void sparkle_main(sparkle_context_t* sparkle)
{
    log_info("Entering main system loop.");
    log_info("Hello, world! I2C baud rate: %d", sparkle->i2c_baudrate);

    while (!usb_control_connected())
    {
        sleep_ms(500);
    }

    log_info("USB Control Port connected.");

    uint8_t buffer[32] = { 0 };
    uint8_t in_char = 0;
    memset(buffer, 'A', sizeof(buffer));
    
    while (true)
    {
        if (usb_control_read(&in_char, 1) > 0)
        {
            if (in_char == 'q')
            {
                usb_control_write(buffer, sizeof(buffer));
            }
            else if (in_char == 'w')
            {
                led_matrix_fill(63);
            }
            else if (in_char == 'e')
            {
                led_matrix_fill(127);
            }
            else if (in_char == 'r')
            {
                led_matrix_fill(191);
            }
            else if (in_char == 't')
            {
                led_matrix_fill(255);
            }
            else if(in_char == 'y')
            {
                led_matrix_clear();
            }
            else if (in_char == 'u')
            {
                log_info("led_matrix(0, 0): %d", led_matrix_get_pixel(0, 0));
            }
        }
        
        sleep_us(1);
    }
}
