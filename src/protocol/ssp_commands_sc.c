#include <pico/stdlib.h>

#include <pico/bootrom.h>
#include <hardware/watchdog.h>

#include "ssp_commands_sc.h"

#include "../pins.h"
#include "../usbcomm/usb_control.h"

_Noreturn ssp_cmd_result_e sc_reboot_bootloader(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    reset_usb_boot(0, 0);

    while (true) {}
}

_Noreturn ssp_cmd_result_e sc_reboot_firmware(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    watchdog_reboot(0, 0, 500);

    while (true) {}
}

ssp_cmd_result_e sc_enter_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    // TODO: Implement sleep timer.

    return SSP_CMD_RESULT_OK;
}

ssp_cmd_result_e sc_exit_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    // TODO: Implement sleep timer.

    return SSP_CMD_RESULT_OK;
}

ssp_cmd_result_e sc_wake_on_command(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    if (*cmd_data) sparkle->state_flags |= SPARKLE_STATEFLAG_WAKE_ON_COMMAND;
    else sparkle->state_flags &= ~SPARKLE_STATEFLAG_WAKE_ON_COMMAND;

    return SSP_CMD_RESULT_OK;
}

ssp_cmd_result_e sc_ignore_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    if (*cmd_data) sparkle->state_flags |= SPARKLE_STATEFLAG_SLEEP_IGNORE;
    else sparkle->state_flags &= ~SPARKLE_STATEFLAG_SLEEP_IGNORE;

    return SSP_CMD_RESULT_OK;
}

ssp_cmd_result_e sc_get_gpio(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    uint8_t pin_status = 0;

    if (pins_get_sleep())
    {
        pin_status |= 0b00000001;
    }

    if (pins_get_dip1())
    {
        pin_status |= 0b00000010;
    }

    if (pins_get_intb())
    {
        pin_status |= 0b00000100;
    }

    if (usb_control_write(&pin_status, 1) < 0)
    {
        return SSP_CMD_RESULT_ERROR;
    }

    return SSP_CMD_RESULT_OK;
}

ssp_cmd_result_e sc_get_state(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    if (usb_control_write(&sparkle->state_flags, 1) < 0)
    {
        return SSP_CMD_RESULT_ERROR;
    }

    return SSP_CMD_RESULT_OK;
}
