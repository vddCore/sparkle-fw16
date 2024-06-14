#include "is3741.h"
#include "log.h"

#include "led_matrix.h"

static is3741_state_t* _is3741 = NULL;

void led_matrix_set_controller(is3741_state_t* is3741)
{
    _is3741 = is3741;
}

void led_matrix_clear(void)
{
    led_matrix_fill(0);
}

void led_matrix_fill(uint8_t brightness)
{
    for(uint8_t y = 0; y < LED_MATRIX_HEIGHT; y++)
    for(uint8_t x = 0; x < LED_MATRIX_WIDTH; x++)
    {
        led_matrix_set_pixel_fast(x, y, brightness);
    }
}

uint8_t led_matrix_get_pixel_fast(uint8_t x, uint8_t y)
{
    uint16_t pixel_info = LED_MATRIX_LUT[LED_MATRIX_XY_TO_LEDREG(x, y)];

    uint8_t pwm_page = (pixel_info & 0x00FF) == 0 ? IS3741_PAGE_PWM0 : IS3741_PAGE_PWM1;
    uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

    /* Why use a local when you can just clobber a parameter lmao. */
    is3741_get_led(_is3741, pixel_id, pwm_page, &x);
    
    return x;
}

void led_matrix_set_pixel_fast(uint8_t x, uint8_t y, uint8_t brightness)
{
    uint16_t pixel_info = LED_MATRIX_LUT[LED_MATRIX_XY_TO_LEDREG(x, y)];

    uint8_t pwm_page = (pixel_info & 0x00FF) == 0 ? IS3741_PAGE_PWM0 : IS3741_PAGE_PWM1;
    uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

    is3741_set_led(_is3741, pixel_id, pwm_page, brightness);
}

uint8_t led_matrix_get_pixel(uint8_t x, uint8_t y)
{
    if (x >= LED_MATRIX_WIDTH)
    {
        log_error("Coordinate X = %d: out of range.", x);
        return 0;
    }

    if (y >= LED_MATRIX_HEIGHT)
    {
        log_error("Coordinate Y = %d: out of range.", y);
        return 0;
    }

    return led_matrix_get_pixel_fast(x, y);
}


void led_matrix_set_pixel(uint8_t x, uint8_t y, uint8_t brightness)
{
    if (x >= LED_MATRIX_WIDTH)
    {
        log_error("Coordinate X = %d: out of range.", x);
        return;
    }

    if (y >= LED_MATRIX_HEIGHT)
    {
        log_error("Coordinate Y = %d: out of range.", y);
        return;
    }

    led_matrix_set_pixel_fast(x, y, brightness);
}
