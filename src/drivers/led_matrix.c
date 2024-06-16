#include <stdlib.h>

#include "drivers/led_matrix.h"
#include "drivers/is3741.h"
#include "debug/log.h"

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
    for (uint8_t y = 0; y < LED_MATRIX_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < LED_MATRIX_WIDTH; x++)
        {
            led_matrix_set_pixel_fast(x, y, brightness);
        }
    }
}

void led_matrix_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t brightness)
{
    int16_t dx = abs(x2 - x1);
    int16_t sx = x1 < x2 ? 1 : -1;

    int16_t dy = -abs(y2 - y1);
    int16_t sy = y1 < y2 ? 1 : -1;

    int16_t err = dx + dy;
    int32_t sqerr;

    while (true)
    {
        if (x1 >= LED_MATRIX_WIDTH || y1 >= LED_MATRIX_HEIGHT)
            break;

        led_matrix_set_pixel_fast(x1, y1, brightness);

        if (x1 == x2 && y1 == y2)
            break;

        sqerr = 2 * err;
        if (sqerr >= dy)
        {
            if (x1 == x2)
                break;

            err += dy;
            x1 += sx;
        }

        if (sqerr <= dx)
        {
            if (y1 == y2)
                break;

            err += dx;
            y1 += sy;
        }
    }
}

void led_matrix_set_pwm_bitmap(uint8_t const* bitmap, size_t len)
{
    if (!bitmap)
    {
        log_error("bitmap == NULL.");
        return;
    }

    if (len != LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
    {
        log_error("Bitmap length is not equal to %d", LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT);
        return;
    }
    
    for (size_t i = 0; i < len; i++)
    {
        uint16_t pixel_info = LED_MATRIX_LUT[i];

        uint8_t pwm_page = (pixel_info & 0x00FF);
        uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

        is3741_set_led_pwm(_is3741, pixel_id, pwm_page, bitmap[i]);
    }
}

void led_matrix_get_pwm_bitmap(uint8_t* buffer)
{
    for (uint8_t i = 0; i < LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT; i++)
    {
        uint16_t pixel_info = LED_MATRIX_LUT[i];

        uint8_t pwm_page = (pixel_info & 0x00FF) == 0 ? IS3741_PAGE_PWM0 : IS3741_PAGE_PWM1;
        uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

        is3741_get_led_pwm(_is3741, pixel_id, pwm_page, buffer + i);
    }
}

void led_matrix_set_dc_scale_bitmap(const uint8_t* bitmap, size_t len)
{
    if (!bitmap)
    {
        log_error("bitmap == NULL.");
        return;
    }

    if (len != LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
    {
        log_error("Bitmap length is not equal to %d", LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT);
        return;
    }

    for (size_t i = 0; i < len; i++)
    {
        uint16_t pixel_info = LED_MATRIX_LUT[i];

        uint8_t pwm_page = (pixel_info & 0x00FF);
        uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

        is3741_set_led_dc_scale(_is3741, pixel_id, pwm_page, bitmap[i]);
    }
}

void led_matrix_get_dc_scale_bitmap(uint8_t* buffer)
{
    for (uint8_t i = 0; i < LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT; i++)
    {
        uint16_t pixel_info = LED_MATRIX_LUT[i];

        uint8_t pwm_page = (pixel_info & 0x00FF) == 0 ? IS3741_PAGE_DC_SCALE0 : IS3741_PAGE_DC_SCALE1;
        uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

        is3741_get_led_dc_scale(_is3741, pixel_id, pwm_page, buffer + i);
    }   
}

uint8_t led_matrix_get_pixel_fast(uint8_t x, uint8_t y)
{
    uint16_t pixel_info = LED_MATRIX_LUT[LED_MATRIX_XY_TO_LEDREG(x, y)];

    uint8_t pwm_page = (pixel_info & 0x00FF) == 0 ? IS3741_PAGE_PWM0 : IS3741_PAGE_PWM1;
    uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

    /* Why use a local when you can just clobber a parameter lmao. */
    is3741_get_led_pwm(_is3741, pixel_id, pwm_page, &x);

    return x;
}

void led_matrix_set_pixel_fast(uint8_t x, uint8_t y, uint8_t brightness)
{
    uint16_t pixel_info = LED_MATRIX_LUT[LED_MATRIX_XY_TO_LEDREG(x, y)];

    uint8_t pwm_page = (pixel_info & 0x00FF) == 0 ? IS3741_PAGE_PWM0 : IS3741_PAGE_PWM1;
    uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

    is3741_set_led_pwm(_is3741, pixel_id, pwm_page, brightness);
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
