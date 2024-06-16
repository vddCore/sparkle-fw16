#include "firmware/kernel.h"
#include "firmware/services/srv_timeout.h"
#include "drivers/pins.h"
#include "debug/log.h"

#include "firmware/services/srv_sleep.h"

static bool _last_ec_sleep_pin;

void srv_sleep_idle_timeout_handler(kernel_context_t* kernel)
{
    if (srv_sleep_is_sleeping(kernel))
        return;
    
    log_info("Timeout reached, entering sleep.");
    srv_sleep_enter(kernel, KERNEL_SLEEPSTATE_TIMEOUT);
}

void srv_sleep_exit(kernel_context_t* kernel)
{
    if (!srv_sleep_is_sleeping(kernel))
    {
        log_warn("Not in a sleep state, ignoring sleep_exit request.");
        return;
    }
    
    log_info("Waking up!");
    
    kernel->state_flags &= ~(KERNEL_STATEFLAG_SLEEPSTATE_MASK);
    pins_set_is3741_sdb(true);

    if (kernel->sleep_timeout_ms > 0)
    {
        srv_timeout_refresh();
        srv_timeout_start(kernel);
    }
}

void srv_sleep_enter(kernel_context_t* kernel, kernel_sleepstate_t state)
{
    if (srv_sleep_is_sleeping(kernel))
    {
        log_warn("Not awake. Ignoring sleep request.");
        return;
    }
    
    if (state == KERNEL_SLEEPSTATE_AWAKE)
    {
        log_error("Attempt to enter awake sleepstate - use sleep_exit instead.");
        return;
    }

    if (kernel->sleep_timeout_ms < 0
        && state != KERNEL_SLEEPSTATE_USB_SUSPEND
        && state != KERNEL_SLEEPSTATE_EC_SLEEP)
    {
        log_info("Ignoring sleep request as per negative timeout value.");
        return;
    }

    log_info("Entering sleep state %02X", state);

    if (srv_timeout_is_running())
        srv_timeout_stop(kernel);

    pins_set_is3741_sdb(false);

    kernel->state_flags &= ~(KERNEL_STATEFLAG_SLEEPSTATE_MASK);
    kernel->state_flags |= state << 1;
}

void srv_sleep_wake_on_command(kernel_context_t* kernel)
{
    if (srv_sleep_is_sleeping(kernel)
        && (kernel->state_flags & KERNEL_STATEFLAG_WAKE_ON_COMMAND_MASK))
    {
        log_info("Triggering wake-on-command.");
        srv_sleep_exit(kernel);
    }
}

bool srv_sleep_is_sleeping(kernel_context_t* kernel)
{
    return kernel->state_flags & KERNEL_STATEFLAG_SLEEPSTATE_MASK;
}

void srv_sleep_monitor_ec_task(kernel_context_t* kernel)
{
    bool sleep_pin = pins_get_sleep();

    if (_last_ec_sleep_pin != sleep_pin)
    {
        if (sleep_pin)
        {
            srv_sleep_exit(kernel);
        }
        else
        {
            srv_sleep_enter(kernel, KERNEL_SLEEPSTATE_EC_SLEEP);
        }

        _last_ec_sleep_pin = sleep_pin;
    }
}
