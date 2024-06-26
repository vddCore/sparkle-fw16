﻿#include <tusb.h>

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED)
#include <device/usbd.h>
#include <device/usbd_pvt.h>
#include <pico/critical_section.h>
#include <pico/mutex.h>
#include <hardware/irq.h>

#include "drivers/usbcomm/usb_task.h"

static mutex_t usb_task_mutex;

static uint8_t low_priority_irq_num;
static critical_section_t one_shot_timer_crit_sec;
static volatile bool one_shot_timer_pending;

static int64_t timer_task(__unused alarm_id_t id, __unused void* user_data)
{
    int64_t repeat_time;
    if (critical_section_is_initialized(&one_shot_timer_crit_sec))
    {
        critical_section_enter_blocking(&one_shot_timer_crit_sec);
        one_shot_timer_pending = false;
        critical_section_exit(&one_shot_timer_crit_sec);
        repeat_time = 0;
    }
    else
    {
        repeat_time = USB_TASK_INTERVAL_US;
    }
    irq_set_pending(low_priority_irq_num);
    return repeat_time;
}

static void low_priority_worker_irq(void)
{
    if (mutex_try_enter(&usb_task_mutex, NULL))
    {
        tud_task();
        mutex_exit(&usb_task_mutex);
    }
    else
    {
        if (critical_section_is_initialized(&one_shot_timer_crit_sec))
        {
            bool need_timer;
            critical_section_enter_blocking(&one_shot_timer_crit_sec);
            need_timer = !one_shot_timer_pending;
            one_shot_timer_pending = true;
            critical_section_exit(&one_shot_timer_crit_sec);

            if (need_timer)
            {
                add_alarm_in_us(USB_TASK_INTERVAL_US, timer_task, NULL, true);
            }
        }
    }
}

static void usb_irq(void)
{
    irq_set_pending(low_priority_irq_num);
}

void usb_task_init(void)
{
    if (get_core_num() != alarm_pool_core_num(alarm_pool_get_default()))
    {
        assert(false);
    }

    tusb_init();
    mutex_init(&usb_task_mutex);

    low_priority_irq_num = (uint8_t)user_irq_claim_unused(true);

    irq_set_exclusive_handler(low_priority_irq_num, low_priority_worker_irq);
    irq_set_enabled(low_priority_irq_num, true);

    if (irq_has_shared_handler(USBCTRL_IRQ))
    {
        irq_add_shared_handler(USBCTRL_IRQ, usb_irq, PICO_SHARED_IRQ_HANDLER_LOWEST_ORDER_PRIORITY);
        critical_section_init_with_lock_num(&one_shot_timer_crit_sec, next_striped_spin_lock_num());
    }
    else
    {
        add_alarm_in_us(USB_TASK_INTERVAL_US, timer_task, NULL, true);
        memset(&one_shot_timer_crit_sec, 0, sizeof(one_shot_timer_crit_sec));
    }
}
#else
#warning TinyUSB is not enabled - USB task will not be initialized.
bool stdio_usb_init(void) {
    return false;
}
#endif // CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED
