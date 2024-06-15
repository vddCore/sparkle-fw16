#ifndef SS_PROTOCOL_H
#define SS_PROTOCOL_H

#include <stdint.h>

#include "../sparkle.h"

#define SSP_CMDGROUP(x) x >> 4;
#define SSP_CMD(x) x & 0x0F;

typedef enum ssp_cmd_result
{
    SSP_CMD_RESULT_OK    = 0,
    SSP_CMD_RESULT_ERROR = -1
} ssp_cmd_result_e;

typedef enum ssp_cmdgroup
{
    SSP_CMDGROUP_SYSTEM_CONTROL           = 0x00,
    SSP_CMDGROUP_CONTROLLER_CONFIGURATION = 0x01,
    SSP_CMDGROUP_DISPLAY_INTERACTION      = 0x02,
    SSP_CMDGROUP_MAX                      = 0x03
} ssp_cmdgroup_e;

typedef enum ssp_sc_cmd
{
    SSP_SC_CMD_REBOOT_BOOTLOADER = 0x00,
    SSP_SC_CMD_REBOOT_FIRMWARE   = 0x01,
    SSP_SC_CMD_ENTER_SLEEP       = 0x02,
    SSP_SC_CMD_EXIT_SLEEP        = 0x03,
    SSP_SC_CMD_WAKE_ON_COMMAND   = 0x04,
    SSP_SC_CMD_IGNORE_SLEEP      = 0x05,
    SSP_SC_CMD_GET_GPIO          = 0x06,
    SSP_SC_CMD_GET_STATE         = 0x07,
    SSP_SC_CMD_MAX               = 0x08
} ssp_sc_cmd_e;

typedef enum ssp_cc_cmd
{
    SSP_CC_CMD_SET_GLOBAL_BRIGHTNESS = 0x00,
    SSP_CC_CMD_GET_GLOBAL_BRIGHTNESS = 0x01,
    SSP_CC_CMD_GET_DIMENSIONS        = 0x02,
    SSP_CC_CMD_MAX                   = 0x03
} ssp_cc_cmd_e;

typedef enum ssp_di_cmd
{
    SSP_DI_CMD_SET_PIXEL   = 0x00,
    SSP_DI_CMD_GET_PIXEL   = 0x01,
    SSP_DI_CMD_DRAW_LINE   = 0x02,
    SSP_DI_CMD_DRAW_BITMAP = 0x03,
    SSP_DI_CMD_MAX         = 0x04
} ssp_di_cmd_e;

typedef ssp_cmd_result_e (*ssp_cmd_handler_t)(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);

#endif // SS_PROTOCOL_H
