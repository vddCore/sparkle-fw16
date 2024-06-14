#include <stdint.h>

#include "../log.h"
#include "../sparkle.h"
#include "../usbcomm/usb_control.h"

#include "sparkle_protocol.h"
#include "ssp_commands_sc.h"

#include "protocol_task.h"

static uint8_t data_buffer[384] = { 0 };

static ssp_cmd_handler_t sc_cmd_registry[SSP_SC_CMD_MAX] = {
    [SSP_SC_CMD_REBOOT_BOOTLOADER] = sc_reboot_bootloader,
    [SSP_SC_CMD_REBOOT_FIRMWARE] = sc_reboot_firmware,
    [SSP_SC_CMD_ENTER_SLEEP] = sc_enter_sleep,
    [SSP_SC_CMD_EXIT_SLEEP] = sc_exit_sleep,
    [SSP_SC_CMD_WAKE_ON_COMMAND] = sc_wake_on_command,
    [SSP_SC_CMD_IGNORE_SLEEP] = sc_ignore_sleep,
    [SSP_SC_CMD_GET_GPIO] = sc_get_gpio,
    [SSP_SC_CMD_GET_STATE] = sc_get_state
};

static ssp_cmd_handler_t cc_cmd_registry[SSP_CC_CMD_MAX] = {
    [SSP_CC_CMD_SET_GLOBAL_BRIGHTNESS] = NULL,
    [SSP_CC_CMD_GET_GLOBAL_BRIGHTNESS] = NULL,
    [SSP_CC_CMD_GET_DIMENSIONS] = NULL
};

static ssp_cmd_handler_t di_cmd_registry[SSP_DI_CMD_MAX] = {
    [SSP_DI_CMD_SET_PIXEL] = NULL,
    [SSP_DI_CMD_GET_PIXEL] = NULL,
    [SSP_DI_CMD_DRAW_LINE] = NULL,
    [SSP_DI_CMD_DRAW_BITMAP] = NULL
};

static void serial_protocol_dispatch_sc_cmd(uint8_t cmd_id, sparkle_context_t* sparkle, int32_t read_count)
{
    if (cmd_id < SSP_SC_CMD_MAX)
    {
        ssp_cmd_handler_t handler = sc_cmd_registry[cmd_id];

        if (handler)
        {
            ssp_cmd_result_e cmd_result;
            if ((cmd_result = handler(sparkle, data_buffer + 1, read_count - 1)) < 0)
            {
                log_error("Command group 0x%02X, command 0x%02X has failed with error code %d.",
                    SSP_CMDGROUP_SYSTEM_CONTROL,
                    cmd_id,
                    cmd_result
                ); 
            }
        }
        else
        {
            log_warn("No handler for command group 0x%02X, command 0x%02X has been defined.",
                SSP_CMDGROUP_SYSTEM_CONTROL,
                cmd_id
            );
        }
    }
}

static void serial_protocol_dispatch_cc_cmd(uint8_t cmd_id, sparkle_context_t* sparkle, int32_t read_count)
{
    if (cmd_id < SSP_CC_CMD_MAX)
    {
        ssp_cmd_handler_t handler = cc_cmd_registry[cmd_id];

        if (handler)
        {
            ssp_cmd_result_e cmd_result;
            if ((cmd_result = handler(sparkle, data_buffer + 1, read_count - 1)) < 0)
            {
                log_error("Command group 0x%02X, command 0x%02X has failed with error code %d.",
                    SSP_CMDGROUP_CONTROLLER_CONFIGURATION,
                    cmd_id,
                    cmd_result
                ); 
            }
        }
        else
        {
            log_warn("No handler for command group 0x%02X, command 0x%02X has been defined.",
                SSP_CMDGROUP_CONTROLLER_CONFIGURATION,
                cmd_id
            );
        }
    }
}

static void serial_protocol_dispatch_di_cmd(uint8_t cmd_id, sparkle_context_t* sparkle, int32_t read_count)
{
    if (cmd_id < SSP_DI_CMD_MAX)
    {
        ssp_cmd_handler_t handler = di_cmd_registry[cmd_id];

        if (handler)
        {
            ssp_cmd_result_e cmd_result;
            if ((cmd_result = handler(sparkle, data_buffer + 1, read_count - 1)) < 0)
            {
                log_error("Command group 0x%02X, command 0x%02X has failed with error code %d.",
                    SSP_CMDGROUP_DISPLAY_INTERACTION,
                    cmd_id,
                    cmd_result
                ); 
            }
        }
        else
        {
            log_warn("No handler for command group 0x%02X, command 0x%02X has been defined.",
                SSP_CMDGROUP_DISPLAY_INTERACTION,
                cmd_id
            );
        }
    }
}

void serial_protocol_task(sparkle_context_t* sparkle)
{
    if (!usb_control_connected())
        return;

    int32_t read_count = -1;
    if ((read_count = usb_control_read(data_buffer, sizeof(data_buffer))) > 0)
    {
        uint8_t cmd_byte = data_buffer[0];

        uint8_t group_id = SSP_CMDGROUP(cmd_byte);
        uint8_t cmd_id = SSP_CMD(cmd_byte);

        if (group_id < SSP_CMDGROUP_MAX)
        {
            switch (group_id)
            {
                case SSP_CMDGROUP_SYSTEM_CONTROL:
                    serial_protocol_dispatch_sc_cmd(cmd_id, sparkle, read_count);
                    break;

                case SSP_CMDGROUP_CONTROLLER_CONFIGURATION:
                    serial_protocol_dispatch_cc_cmd(cmd_id, sparkle, read_count);
                    break;

                case SSP_CMDGROUP_DISPLAY_INTERACTION:
                    serial_protocol_dispatch_di_cmd(cmd_id, sparkle, read_count);
                    break;

                default: break;
            }
        }
    }
}
