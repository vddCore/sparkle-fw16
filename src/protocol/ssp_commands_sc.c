#include <pico/stdlib.h>
#include <pico/bootrom.h>

#include <hardware/resets.h>
#include <hardware/sync.h>

#include "ssp_commands_sc.h"

_Noreturn ssp_cmd_result_e sc_reboot_bootloader(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    reset_usb_boot(0, 0);
    
    while(true)
    {
    }
}

_Noreturn ssp_cmd_result_e sc_reboot_firmware(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    uint32_t irq_state = save_and_disable_interrupts();

    reset_block(
        RESETS_RESET_BITS
        & ~RESETS_RESET_IO_BANK0_BITS
        & ~RESETS_RESET_PADS_BANK0_BITS
    );

    restore_interrupts(irq_state);

    while (true)
    {
        __wfi();
    }
}

ssp_cmd_result_e sc_enter_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    
}

ssp_cmd_result_e sc_exit_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    
}

ssp_cmd_result_e sc_wake_on_command(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    
}

ssp_cmd_result_e sc_ignore_sleep(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    
}

ssp_cmd_result_e sc_get_gpio(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    
}

ssp_cmd_result_e sc_get_state(sparkle_context_t* sparkle, uint8_t* cmd_data, size_t cmd_data_len)
{
    
}

