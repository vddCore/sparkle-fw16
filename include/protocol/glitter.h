#ifndef GLITTER_H
#define GLITTER_H

typedef struct glitter_device_info
{
    uint8_t sleep_pin;         /* 1 = on, 0 = off */
    uint8_t dip1_pin;          /* 1 = on, 0 = off */
    uint8_t intb_pin;          /* 1 = on, 0 = off */
    uint8_t state_flags;       /* see firmware/kernel.h for details */
    uint8_t id_reg;            /* see IS31FL3741A datasheet for details */
    uint8_t config_reg;
    uint8_t global_brightness;
    uint8_t display_width;     /* 9 for the official LED matrix module */
    uint8_t display_height;    /* 34 for the official LED matrix module */
} glitter_device_info_t;

typedef enum glitter_basic_command
{
    GLITTER_CMD_REBOOT                = 0x00,
    GLITTER_CMD_SLEEP                 = 0x01,
    GLITTER_CMD_SET_WAKE_ON_COMMAND   = 0x02,
    GLITTER_CMD_SET_SLEEP_TIMEOUT     = 0x03,
    GLITTER_CMD_SET_GLOBAL_BRIGHTNESS = 0x04,
    GLITTER_CMD_DRAW_PIXEL            = 0x05,
    GLITTER_CMD_DRAW_LINE             = 0x06
} __attribute__((packed)) glitter_basic_command_t;

typedef enum glitter_cmd_reboot_target
{
    GLITTER_CMD_REBOOT_BOOTLOADER = 0x00,
    GLITTER_CMD_REBOOT_FIRMWARE   = 0x01
} __attribute__((packed)) glitter_cmd_reboot_target_t;

typedef enum glitter_sleep_directive
{
    GLITTER_CMD_SLEEP_EXIT  = 0x00,
    GLITTER_CMD_SLEEP_ENTER = 0x01
} __attribute__((packed)) glitter_cmd_sleep_directive_t;

#endif
