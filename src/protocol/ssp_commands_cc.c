#include "../is3741.h"
#include "../led_matrix.h"

#include "ssp_commands_cc.h"

#include "../usbcomm/usb_control.h"

ssp_cmd_result_e cc_set_global_brightness(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    if (is3741_set_global_current(sparkle->is3741, *cmd_data) < 0)
    {
        return SSP_CMD_RESULT_ERROR;
    }
    
    return SSP_CMD_RESULT_OK;
}

ssp_cmd_result_e cc_get_global_brightness(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    ssp_cmd_result_e result = SSP_CMD_RESULT_OK;
    uint8_t brightness = 0;

    if (is3741_get_global_current(sparkle->is3741, &brightness) < 0)
    {
        result = SSP_CMD_RESULT_ERROR;
    }

    if (usb_control_write(&brightness, 1) < 0)
    {
        result = SSP_CMD_RESULT_ERROR;
    }

    return result;
}

ssp_cmd_result_e cc_get_dimensions(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    uint8_t dimensions[2] = { LED_MATRIX_WIDTH, LED_MATRIX_HEIGHT };

    if (usb_control_write(dimensions, 2) < 0)
    {
        return SSP_CMD_RESULT_ERROR;
    }
    
    return SSP_CMD_RESULT_OK;
}
