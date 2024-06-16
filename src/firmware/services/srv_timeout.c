#include <hardware/timer.h>

#include "debug/log.h"
#include "firmware/kernel.h"

#include "firmware/services/srv_timeout.h"

static bool _is_running = false;

static srv_timeout_event_t _timeout_handlers[SRV_TIMEOUT_NOTIFICATION_MAX] = { 0 };

static bool _raising_timeouts = false;
static bool _timeouts_raised = false;
static uint64_t _last_command_stamp = 0;

int8_t srv_timeout_add_event_handler(srv_timeout_event_t timeout_handler)
{
    int8_t slot = -1;

    for (int8_t i = 0; i < SRV_TIMEOUT_NOTIFICATION_MAX; i++)
    {
        if (_timeout_handlers[i] == NULL)
        {
            slot = i;
            break;            
        }
    }
    
    if (slot < 0)
    {
        log_error("No free notification slots.");
    }
    else
    {
        log_info("Registeringg timeout handler in slot %d", slot);
        _timeout_handlers[slot] = timeout_handler;        
    }

    return slot;
}

void srv_timeout_refresh(void)
{
    if (_raising_timeouts)
    {
        log_error("Attempt to refresh timeouts while notifications are being raised.");
        return;
    }
    
    _last_command_stamp = time_us_64();
    _timeouts_raised = false;
}

void srv_timeout_start(kernel_context_t* kernel)
{
    _is_running = true;
}

void srv_timeout_stop(kernel_context_t* kernel)
{
    _is_running = false;
}

bool srv_timeout_is_running(void)
{
    return _is_running;
}

void srv_timeout_task(kernel_context_t* kernel)
{
    if (!_is_running)
        return;

    if (kernel->sleep_timeout_ms <= 0)
        return;
    
    if (time_us_64() > (_last_command_stamp + (kernel->sleep_timeout_ms * 1000)) && !_timeouts_raised)
    {
        _raising_timeouts = true;
        
        for (uint8_t i = 0; i < SRV_TIMEOUT_NOTIFICATION_MAX; i++)
        {
            if (_timeout_handlers[i])
                _timeout_handlers[i](kernel);
        }

        _raising_timeouts = false;
        _timeouts_raised = true;
    }
}