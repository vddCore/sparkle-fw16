#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <pico/stdlib.h>

#include "debug/log.h"
#include "drivers/pins.h"
#include "drivers/is3741.h"
#include "drivers/led_matrix.h"

#include "firmware/kernel.h"

#include <firmware/services/srv_sleep.h>

#include "firmware/services/srv_timeout.h"

static kernel_context_t _kernel = { 0 };

static int32_t kernel_led_matrix_init(void)
{
    int32_t result = (int32_t)is3741_init(IS3741_I2C_ADDR, &_kernel.is3741);

#if LED_MATRIX_USE_EVT_LUT
    if (!result) result = (int32_t)is3741_set_sws_config(kernel->is3741, IS3741_SWS_SW1SW9);
#else
    if (!result) result = (int32_t)is3741_set_sws_config(_kernel.is3741, IS3741_SWS_SW1SW8);
#endif

    if (!result) result = is3741_set_led_dc_scale_global(_kernel.is3741, IS3741_DC_SCALE_DEFAULT);
    if (!result) result = is3741_set_pwm_freq(_kernel.is3741, IS3741_PFS_29000HZ);

    if (!result)
    {
        led_matrix_set_controller(_kernel.is3741);
        led_matrix_clear();
    }

    pins_set_is3741_sdb(true);
    return result;
}

kernel_context_t* kernel_init(void)
{
    if (kernel_led_matrix_init() < 0)
    {
        kernel_panic("Unable to initialize LED matrix.");
    }

    srv_timeout_add_event_handler(&srv_sleep_idle_timeout_handler);
    return &_kernel;
}

_Noreturn void kernel_panic(const char* why)
{
    while (true)
    {
        log_error("PANIC! %s", why);
        sleep_ms(KERNEL_PANIC_INTERVAL_MS);
    }
}

_Noreturn void kernel_main(void)
{
    log_info("Entering main system loop.");

    while (true)
    {
        srv_timeout_task(&_kernel);
        srv_sleep_monitor_ec_task(&_kernel);
    }
}
