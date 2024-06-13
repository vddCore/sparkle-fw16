﻿#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hardware/i2c.h>

#include "log.h"
#include "is3741.h"

#include "sparkle.h"

#define i2c_write_stop(state, buf) i2c_write_blocking(i2c1, state->i2c_address, buf, sizeof(buf), false)
#define i2c_write_stop_sz(state, buf, sz) i2c_write_blocking(i2c1, state->i2c_address, buf, sz, false)
#define i2c_write_nostop(state, buf) i2c_write_blocking(i2c1, state->i2c_address, buf, sizeof(buf), true)

typedef struct is3741_state
{
    uint8_t i2c_address;
    uint8_t current_page;
} is3741_state_t;

is3741_err_t is3741_init(uint8_t i2c_address, is3741_state_t** out_state)
{
    is3741_state_t* new_state = (is3741_state_t*)calloc(1, sizeof(is3741_state_t));

    if (!new_state)
    {
        log_error("Unable to allocate memory for a new IS3741 state structure.");
        return IS3741_ERR_OUT_OF_MEMORY;
    }

    new_state->i2c_address = i2c_address;

    uint8_t id_register = 0;
    if (is3741_read_id_register(new_state, &id_register) < 0)
    {
        log_error("Initialization failure (ID).");
        goto __failure;
    }

    if (id_register != IS3741_ID)
    {
        log_error("Initialization failure (ID mismatch).");
        goto __failure;
    }

    if (is3741_reset(new_state) < 0)
    {
        log_error("Initialization failure (reset).");
        goto __failure;
    }

    if (is3741_enable(new_state, false) < 0)
    {
        log_error("Initialization failure (shutdown).");
        goto __failure;
    }
    
    if (is3741_set_global_current(new_state, 255) < 0)
    {
        log_error("Initialization failure (current).");
        goto __failure;
    }
    
    if (is3741_enable(new_state, true) < 0)
    {
        log_error("Initialization failure (re-enable).");
        goto __failure;
    }
    
    *out_state = new_state;
    return IS3741_ERR_OK;

__failure:
    free(new_state);
    return IS3741_ERR_GENERIC;
}

void is3741_exit(is3741_state_t* state)
{
    if (!state)
    {
        log_error("Attempt to destroy a NULL state.");
        return;
    }

    free(state);
}

is3741_err_t is3741_set_pixel(is3741_state_t* state, uint8_t x, uint8_t y, uint8_t brightness)
{
    if (x >= LED_MATRIX_WIDTH)
    {
        log_error("Coordinate X = %d: out of range.", x);
        return IS3741_ERR_COORD_X;
    }

    if (y >= LED_MATRIX_HEIGHT)
    {
        log_error("Coordinate Y = %d: out of range.", y);
        return IS3741_ERR_COORD_Y;
    }

    uint16_t pixel_info = LUT_PIXEL[IS3741_XY_TO_LEDREG(x, y)];

    uint8_t pwm_page = (pixel_info & 0x00FF) == 0 ? IS3741_PAGE_PWM0 : IS3741_PAGE_PWM1;
    uint8_t pixel_id = (pixel_info & 0xFF00) >> 8;

    if (is3741_select_page(state, pwm_page) < 0)
    {
        log_error("Unable to select PWM page %d.", pwm_page);
        return IS3741_ERR_PAGESELECT;
    }

    uint8_t data[] = { pixel_id, brightness };
    if (i2c_write_stop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }

    return IS3741_ERR_OK;
}

is3741_err_t is3741_unlock_crwl(is3741_state_t* state)
{
    uint8_t data[] = { IS3741_REG_CRWL, 0b11000101 };
    if (i2c_write_stop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }

    return IS3741_ERR_OK;
}

is3741_err_t is3741_select_page(is3741_state_t* state, uint8_t page)
{
    if (page >= IS3741_PAGE_MAX)
    {
        log_error("Attempt to select page register %d, which is out of range.", page);
        return IS3741_ERR_PAGESELECT;
    }

    if (page == state->current_page)
    {
        /**
         * An attempt to set a page to the
         * currently selected one is a no-op.
         **/
        
        return IS3741_ERR_OK;
    }
    
    if (is3741_unlock_crwl(state) < 0)
    {
        log_error("Unable to unlock CRWL register.");
        return IS3741_ERR_CRWL;
    }

    uint8_t data[] = { IS3741_REG_CR, page };
    if (i2c_write_stop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }

    state->current_page = page;
    return IS3741_ERR_OK;
}

is3741_err_t is3741_reset(is3741_state_t* state)
{
    if (is3741_select_page(state, IS3741_PAGE_FUNCTION) < 0)
    {
        log_error("Unable to select the function page.");
        return IS3741_ERR_PAGESELECT;
    }
    
    uint8_t data[] = { IS3741_FUNCREG_RESET, 0xAE };
    if (i2c_write_stop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }

    sleep_ms(10);
    
    return IS3741_ERR_OK;
}

is3741_err_t is3741_enable(is3741_state_t* state, bool enable)
{
    if (is3741_select_page(state, IS3741_PAGE_FUNCTION) < 0)
    {
        log_error("Unable to select the function page.");
        return IS3741_ERR_PAGESELECT;
    }
    
    uint8_t cfg_byte = 0;
    if (is3741_read_config_register(state, &cfg_byte) < 0)
    {
        log_error("Unable to read the configuration register.");
        return IS3741_ERR_REG_READ;
    }

    if (enable)
    {
        cfg_byte |= IS3741_CONFIG_SSD;    
    }
    else
    {
        cfg_byte &= ~(IS3741_CONFIG_SSD);
    }

    uint8_t data[] = { IS3741_FUNCREG_CONFIG, cfg_byte };
    if (i2c_write_stop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }
    
    return IS3741_ERR_OK;
}

is3741_err_t is3741_set_sws_config(is3741_state_t* state, uint8_t sws_value)
{
    if (is3741_select_page(state, IS3741_PAGE_FUNCTION) < 0)
    {
        log_error("Unable to select the function page.");
        return IS3741_ERR_PAGESELECT;
    }

    uint8_t cfg_byte = 0;
    if (is3741_read_config_register(state, &cfg_byte) < 0)
    {
        log_error("Unable to read the configuration register.");
        return IS3741_ERR_REG_READ;
    }

    cfg_byte |= sws_value;

    uint8_t data[] = { IS3741_FUNCREG_CONFIG, cfg_byte };
    if (i2c_write_stop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }
    
    return IS3741_ERR_OK;
}

is3741_err_t is3741_set_pwm_freq(is3741_state_t* state, uint8_t pfs_value)
{
    if (is3741_select_page(state, IS3741_PAGE_FUNCTION) < 0)
    {
        log_error("Unable to select the function page.");
        return IS3741_ERR_PAGESELECT;
    }

    uint8_t data[] = { IS3741_FUNCREG_PWM_FREQ, pfs_value };
    if (i2c_write_stop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }

    return IS3741_ERR_OK;
}

is3741_err_t is3741_set_global_current(is3741_state_t* state, uint8_t brightness)
{
    if (is3741_select_page(state, IS3741_PAGE_FUNCTION) < 0)
    {
        log_error("Unable to select the function page.");
        return IS3741_ERR_PAGESELECT;
    }

    uint8_t data[] = { IS3741_FUNCREG_GLOBAL_CURRENT, brightness };
    if (i2c_write_stop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }

    return IS3741_ERR_OK;
}

is3741_err_t is3741_set_led_scaling(is3741_state_t* state, uint8_t brightness)
{
    if (is3741_select_page(state, IS3741_PAGE_LED_SCALING1) < 0)
    {
        log_error("Unable to select the function page (1/2).");
        return IS3741_ERR_PAGESELECT;
    }

    /* +1 for the initial LED index. */
    uint8_t led_scale_all[IS3741_LED_SCALE1_SZ + 1];
    memset(led_scale_all, brightness, IS3741_LED_SCALE1_SZ + 1);
    led_scale_all[0] = 0;
    
    if (i2c_write_stop(state, led_scale_all) < 0)
    {
        log_error("I2C write error (1/2).");
        return IS3741_ERR_I2C_WRITE;
    }

    if (is3741_select_page(state, IS3741_PAGE_LED_SCALING2) < 0)
    {
        log_error("Unable to select the function page (2/2).");
        return IS3741_ERR_PAGESELECT;
    }

    if (i2c_write_stop_sz(state, led_scale_all, IS3741_LED_SCALE2_SZ + 1) < 0)
    {
        log_error("I2C write error (2/2).");
        return IS3741_ERR_I2C_WRITE;
    }

    return IS3741_ERR_OK;
}

is3741_err_t is3741_read_config_register(is3741_state_t* state, uint8_t* cfg_byte)
{
    if (is3741_select_page(state, IS3741_PAGE_FUNCTION) < 0)
    {
        log_error("Unable to select the function page.");
        return IS3741_ERR_PAGESELECT;
    }
    
    if (is3741_read_reg8(state, IS3741_FUNCREG_CONFIG, cfg_byte))
    {
        log_error("Unable to read the configuration register.");
        return IS3741_ERR_REG_READ;
    }

    return IS3741_ERR_OK;
}

is3741_err_t is3741_read_id_register(is3741_state_t* state, uint8_t* id_byte)
{
    if (is3741_read_reg8(state, IS3741_REG_ID, id_byte) < 0)
    {
        log_error("Unable to read the ID register.");
        return IS3741_ERR_REG_READ;
    }

    return IS3741_ERR_OK;
}

is3741_err_t is3741_read_reg8(is3741_state_t* state, uint8_t reg, uint8_t* out_data)
{
    uint8_t data[] = { reg };
    if (i2c_write_nostop(state, data) < 0)
    {
        log_error("I2C write error.");
        return IS3741_ERR_I2C_WRITE;
    }

    if (i2c_read_blocking(SPARKLE_I2C_INSTANCE, state->i2c_address, out_data, 1, false) < 0)
    {
        log_error("I2C read error.");
        return IS3741_ERR_I2C_READ;
    }

    return IS3741_ERR_OK;
}