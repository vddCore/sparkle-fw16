#ifndef SSP_COMMANDS_CC_H
#define SSP_COMMANDS_CC_H

#include "../sparkle.h"

#include "sparkle_protocol.h"

ssp_cmd_result_e cc_set_global_brightness(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
ssp_cmd_result_e cc_get_global_brightness(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);
ssp_cmd_result_e cc_get_dimensions(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len);

#endif //SSP_COMMANDS_CC_H
