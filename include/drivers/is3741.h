﻿#ifndef IS3741_H
#define IS3741_H

#include <stdint.h>
#include <stdbool.h>

#define IS3741_I2C_ADDR 0x30
#define IS3741_ID 0x60

#define IS3741_PAGE_PWM0              0x00
#define IS3741_PAGE_PWM1              0x01
#define IS3741_PAGE_DC_SCALE0         0x02
#define IS3741_PAGE_DC_SCALE1         0x03
#define IS3741_PAGE_FUNCTION          0x04
#define IS3741_PAGE_MAX               0x05

#define IS3741_REG_IMR      0xF0 /* Interrupt Mask Register */
#define IS3741_REG_ISR      0xF1 /* Interrupt Status Register */
#define IS3741_REG_ID       0xFC /* Identification Register */
#define IS3741_REG_CR       0xFD /* Command Register */
#define IS3741_REG_CRWL     0xFE /* Command Register Write Lock */

#define IS3741_FUNCREG_CONFIG         0x00
#define IS3741_FUNCREG_GLOBAL_CURRENT 0x01
#define IS3741_FUNCREG_PWM_FREQ       0x36
#define IS3741_FUNCREG_RESET          0x3F

#define IS3741_CONFIG_SSD        0b00000001
#define IS3741_CONFIG_OSDE_OPEN1 0b00000010
#define IS3741_CONFIG_OSDE_OPEN2 0b00000110
#define IS3741_CONFIG_OSDE_SHORT 0b00000100
#define IS3741_CONFIG_LGCV_24_06 0b00001000

#define IS3741_SWS_SW1SW9 0b00000000
#define IS3741_SWS_SW1SW8 0b00010000
#define IS3741_SWS_SW1SW7 0b00100000
#define IS3741_SWS_SW1SW6 0b00110000
#define IS3741_SWS_SW1SW5 0b01000000
#define IS3741_SWS_SW1SW4 0b01010000
#define IS3741_SWS_SW1SW3 0b01100000
#define IS3741_SWS_SW1SW2 0b01110000
#define IS3741_SWS_NOSCAN 0b10000000

#define IS3741_PFS_29000HZ 0b00000000
#define IS3741_PFS_3600HZ  0b00000011
#define IS3741_PFS_1800HZ  0b00000111
#define IS3741_PFS_900HZ   0b00001011

#define IS3741_LED_PAGE0_SIZE 0xB4
#define IS3741_LED_PAGE1_SIZE 0xAB

#define IS3741_DC_SCALE_DEFAULT 40

typedef struct is3741_state is3741_state_t;

typedef enum is3741_err
{
    IS3741_ERR_OK = 0,
    IS3741_ERR_GENERIC = -1,
    IS3741_ERR_OUT_OF_MEMORY = -2,
    IS3741_ERR_I2C_READ = -3,
    IS3741_ERR_I2C_WRITE = -4,
    IS3741_ERR_PAGESELECT = -5,
    IS3741_ERR_CRWL = -6,
    IS3741_ERR_REG_READ = -7,
    IS3741_ERR_STATE = -8
} is3741_err_t;

is3741_err_t is3741_init(uint8_t i2c_address, is3741_state_t** out_state);
void is3741_exit(is3741_state_t* state);
is3741_err_t is3741_set_led_pwm(is3741_state_t* state, uint8_t led_id, uint8_t pwm_page, uint8_t value);
is3741_err_t is3741_get_led_pwm(is3741_state_t* state, uint8_t led_id, uint8_t pwm_page, uint8_t* out_value);
is3741_err_t is3741_set_led_dc_scale(is3741_state_t* state, uint8_t led_id, uint8_t pwm_page, uint8_t value);
is3741_err_t is3741_get_led_dc_scale(is3741_state_t* state, uint8_t led_id, uint8_t pwm_page, uint8_t* out_value);
is3741_err_t is3741_set_led_dc_scale_global(is3741_state_t* state, uint8_t value);
is3741_err_t is3741_unlock_crwl(is3741_state_t* state);
is3741_err_t is3741_select_page(is3741_state_t* state, uint8_t page);
is3741_err_t is3741_reset(is3741_state_t* state);
is3741_err_t is3741_enable(is3741_state_t* state, bool enable);
is3741_err_t is3741_set_sws_config(is3741_state_t* state, uint8_t sws_value);
is3741_err_t is3741_set_pwm_freq(is3741_state_t* state, uint8_t pfs_value);
is3741_err_t is3741_get_global_current(is3741_state_t* state, uint8_t* out_brightness);
is3741_err_t is3741_set_global_current(is3741_state_t* state, uint8_t brightness);
is3741_err_t is3741_read_config_register(is3741_state_t* state, uint8_t* cfg_byte);
is3741_err_t is3741_read_id_register(is3741_state_t* state, uint8_t* id_byte);
is3741_err_t is3741_read_reg8(is3741_state_t* state, uint8_t reg, uint8_t* out_data);

#endif // IS3741_H
