#include <stdlib.h>
#include <math.h>

#include <pico/platform.h>

#include "is3741.h"
#include "log.h"

#include "led_matrix.h"

static is3741_state_t* _is3741 = NULL;

void led_matrix_set_controller(is3741_state_t* is3741)
{
    _is3741 = is3741;
}

void led_matrix_line(
    uint8_t x1, uint8_t y1,
    uint8_t x2, uint8_t y2,
    uint8_t brightness)
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

void led_matrix_triangle_stroke(
    uint8_t x1, uint8_t y1,
    uint8_t x2, uint8_t y2,
    uint8_t x3, uint8_t y3,
    uint8_t brightness)
{
    led_matrix_line(x1, y1, x2, y2, brightness);
    led_matrix_line(x1, y1, x3, y3, brightness);
    led_matrix_line(x2, y2, x3, y3, brightness);
}

void led_matrix_triangle_fill(
    uint8_t x1, uint8_t y1,
    uint8_t x2, uint8_t y2,
    uint8_t x3, uint8_t y3,
    uint8_t brightness)
{
    led_matrix_triangle_stroke(x1, y1, x2, y2, x3, y3, brightness);
    
    int16_t dx12 = x1 - x2;
    int16_t dx23 = x2 - x3;
    int16_t dx31 = x3 - x1;

    int16_t dy12 = y1 - y2;
    int16_t dy23 = y2 - y3;
    int16_t dy31 = y3 - y1;

    uint8_t minx = (uint8_t)MIN(x1, MIN(x2, x3));
    uint8_t maxx = (uint8_t)MAX(x1, MAX(x2, x3));
    uint8_t miny = (uint8_t)MIN(y1, MIN(y2, y3));
    uint8_t maxy = (uint8_t)MAX(y1, MAX(y2, y3));

    int16_t C1 = dy12 * x1 - dx12 * y1;
    int16_t C2 = dy23 * x2 - dx23 * y2;
    int16_t C3 = dy31 * x3 - dx31 * y3;

    int16_t Cy1 = C1 + dx12 * miny - dy12 * minx;
    int16_t Cy2 = C2 + dx23 * miny - dy23 * minx;
    int16_t Cy3 = C3 + dx31 * miny - dy31 * minx;

    for (uint8_t y = miny; y < maxy; y++)
    {
        int16_t Cx1 = Cy1;
        int16_t Cx2 = Cy2;
        int16_t Cx3 = Cy3;

        for (uint8_t x = minx; x < maxx; x++)
        {
            if (Cx1 > 0 && Cx2 > 0 && Cx3 > 0)
            {
                led_matrix_set_pixel_fast(x, y, brightness);
            }

            Cx1 -= dy12;
            Cx2 -= dy23;
            Cx3 -= dy31;
        }

        Cy1 += dx12;
        Cy2 += dx23;
        Cy3 += dx31;
    } 
}

void led_matrix_rect_stroke(
    uint8_t x1, uint8_t y1,
    uint8_t x2, uint8_t y2,
    uint8_t brightness)
{
    led_matrix_line(x1, y1, x2, y1, brightness); 
    led_matrix_line(x1, y2, x2, y2, brightness);
    led_matrix_line(x1, y1 + 1, x1, y2 - 1, brightness);
    led_matrix_line(x2, y1 + 1, x2, y2 - 1, brightness);
}

void led_matrix_rect_fill(
    uint8_t x1, uint8_t y1,
    uint8_t x2, uint8_t y2,
    uint8_t brightness)
{
    uint8_t l = MIN(x1, x2),
            t = MIN(y1, y2),
            r = MAX(x1, x2),
            b = MAX(y1, y2);

    uint8_t width = r - l,
            height = b - t;

    for (uint8_t y = 0; y <= height; y++)
    {
        for (uint8_t x = 0; x <= width; x++)
        {
            led_matrix_set_pixel_fast(l + x, t + y, brightness);
        }
    }
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
