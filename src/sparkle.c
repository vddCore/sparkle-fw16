#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "sparkle.h"

#include "log.h"
#include "is3741.h"
#include "led_matrix.h"
#include "usbcomm/usb_control.h"
#include "usbcomm/usb_stdio.h"

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
    if (!result)
        result = (int32_t)is3741_set_sws_config(
            sparkle->is3741, IS3741_SWS_SW1SW8);
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
    while (!usb_stdio_connected())
    {
        sleep_ms(500);
    }
    
    log_info("Entering main system loop.");
    log_info("Hello, world! I2C baud rate: %d", sparkle->i2c_baudrate);

    uint8_t buffer[32] = { 0 };
    uint8_t in_char = 0;
    memset(buffer, 'A', sizeof(buffer));
    
    while (true)
    {
        if (usb_control_read(&in_char, 1) > 0)
        {
            switch (in_char)
            {
                case 'q':
                {
                    usb_control_write(buffer, sizeof(buffer));
                    break;
                }
                
                case 'w':
                {
                    led_matrix_fill(63);
                    break;
                }
                
                case 'e':
                {
                    led_matrix_fill(127);
                    break;
                }
                
                case 'r':
                {
                    led_matrix_fill(191);
                    break;
                }
                
                case 't':
                {
                    led_matrix_fill(255);
                    break;
                }
                
                case 'y':
                {
                    led_matrix_clear();
                    break;
                }
                
                case 'u':
                {
                    for (uint8_t y = 0; y < LED_MATRIX_HEIGHT; y++)
                    {
                        printf("%02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
                            led_matrix_get_pixel(0, y),
                            led_matrix_get_pixel(1, y),
                            led_matrix_get_pixel(2, y),
                            led_matrix_get_pixel(3, y),
                            led_matrix_get_pixel(4, y),
                            led_matrix_get_pixel(5, y),
                            led_matrix_get_pixel(6, y),
                            led_matrix_get_pixel(7, y),
                            led_matrix_get_pixel(8, y)
                        );
                    }
                    
                    break;
                }
            }
        }

        sleep_us(1);
    }
}
