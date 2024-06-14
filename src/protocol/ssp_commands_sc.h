#ifndef SSP_COMMANDS_SC_H
#define SSP_COMMANDS_SC_H

#include "sparkle_protocol.h"

_Noreturn ssp_cmd_result_e sc_reboot_bootloader(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
_Noreturn ssp_cmd_result_e sc_reboot_firmware(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
ssp_cmd_result_e sc_enter_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
ssp_cmd_result_e sc_exit_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
ssp_cmd_result_e sc_wake_on_command(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
ssp_cmd_result_e sc_ignore_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
ssp_cmd_result_e sc_get_gpio(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
ssp_cmd_result_e sc_get_state(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);

#endif //SSP_COMMANDS_SC_H
