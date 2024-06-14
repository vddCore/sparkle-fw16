#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>
#include <stddef.h>

#include "is3741.h"

#ifdef SPARKLE_USE_EVT_LUT
    #define LED_MATRIX_USE_EVT_LUT 1
#else
#define LED_MATRIX_USE_EVT_LUT 0
#endif

#define LED_MATRIX_WIDTH  9
#define LED_MATRIX_HEIGHT 34

#define LED_MATRIX_XY_TO_LEDREG(x, y) y * LED_MATRIX_WIDTH + x

/* def. in led_matrix_lut.c */
extern const uint16_t LED_MATRIX_LUT[LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT];

void led_matrix_set_controller(is3741_state_t* is3741);

void led_matrix_clear(void);
void led_matrix_fill(uint8_t brightness);

void led_matrix_line(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2, uint8_t brightness);
void led_matrix_bitmap(const uint8_t* bitmap, size_t len);

uint8_t led_matrix_get_pixel_fast(uint8_t x, uint8_t y);
void led_matrix_set_pixel_fast(uint8_t x, uint8_t y, uint8_t brightness);

uint8_t led_matrix_get_pixel(uint8_t x, uint8_t y);
void led_matrix_set_pixel(uint8_t x, uint8_t y, uint8_t brightness);

#endif //LED_MATRIX_H
