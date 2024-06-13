#include <tusb.h>

#if (CFG_TUD_ENABLED | TUSB_OPT_DEVICE_ENABLED) && CFG_TUD_CDC

#include <pico/binary_info.h>
#include <pico/time.h>
#include <pico/stdio/driver.h>
#include <pico/mutex.h>
#include <hardware/irq.h>
#include <device/usbd_pvt.h>

#include "usb_stdio.h"

static mutex_t stdio_usb_mutex;

#if USB_STDIO_USB_SUPPORT_CHARS_AVAILABLE_CALLBACK
static void (*chars_available_callback)(void*);
static void* chars_available_param;
#endif

static critical_section_t one_shot_timer_crit_sec;
static volatile bool one_shot_timer_pending;

static uint8_t low_priority_irq_num;

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
        repeat_time = USB_STDIO_TASK_INTERVAL_US;
    }
    irq_set_pending(low_priority_irq_num);
    return repeat_time;
}

static void low_priority_worker_irq(void)
{
    if (mutex_try_enter(&stdio_usb_mutex, NULL))
    {
        tud_task();
        mutex_exit(&stdio_usb_mutex);
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
                add_alarm_in_us(USB_STDIO_TASK_INTERVAL_US, timer_task, NULL, true);
            }
        }
    }
}

static void usb_irq(void)
{
    irq_set_pending(low_priority_irq_num);
}

static void stdio_usb_out_chars(const char* buf, int length)
{
    static uint64_t last_avail_time;
    if (!mutex_try_enter_block_until(&stdio_usb_mutex, make_timeout_time_ms(PICO_STDIO_DEADLOCK_TIMEOUT_MS)))
    {
        return;
    }
    if (usb_stdio_connected())
    {
        for (int i = 0; i < length;)
        {
            int n = length - i;
            int avail = (int)tud_cdc_write_available();
            if (n > avail) n = avail;
            if (n)
            {
                int n2 = (int)tud_cdc_write(buf + i, (uint32_t)n);
                tud_task();
                tud_cdc_write_flush();
                i += n2;
                last_avail_time = time_us_64();
            }
            else
            {
                tud_task();
                tud_cdc_write_flush();
                if (!usb_stdio_connected() ||
                    (!tud_cdc_write_available() && time_us_64() > last_avail_time + USB_STDIO_STDOUT_TIMEOUT_US))
                {
                    break;
                }
            }
        }
    }
    else
    {
        last_avail_time = 0;
    }
    mutex_exit(&stdio_usb_mutex);
}

int stdio_usb_in_chars(char* buf, int length)
{
    int rc = PICO_ERROR_NO_DATA;
    if (usb_stdio_connected() && tud_cdc_available())
    {
        if (!mutex_try_enter_block_until(&stdio_usb_mutex, make_timeout_time_ms(PICO_STDIO_DEADLOCK_TIMEOUT_MS)))
        {
            return PICO_ERROR_NO_DATA;
        }
        if (usb_stdio_connected() && tud_cdc_available())
        {
            int count = (int)tud_cdc_read(buf, (uint32_t)length);
            rc = count ? count : PICO_ERROR_NO_DATA;
        }
        else
        {
            tud_task();
        }
        mutex_exit(&stdio_usb_mutex);
    }
    return rc;
}

#if USB_STDIO_USB_SUPPORT_CHARS_AVAILABLE_CALLBACK
void tud_cdc_rx_cb(__unused uint8_t itf)
{
    if (chars_available_callback)
    {
        usbd_defer_func(chars_available_callback, chars_available_param, false);
    }
}

void stdio_usb_set_chars_available_callback(void (*fn)(void*), void* param)
{
    chars_available_callback = fn;
    chars_available_param = param;
}
#endif

stdio_driver_t stdio_usb = {
    .out_chars = stdio_usb_out_chars,
    .in_chars = stdio_usb_in_chars,
#if USB_STDIO_USB_SUPPORT_CHARS_AVAILABLE_CALLBACK
    .set_chars_available_callback = stdio_usb_set_chars_available_callback,
#endif
#if PICO_STDIO_ENABLE_CRLF_SUPPORT
    .crlf_enabled = USB_STDIO_DEFAULT_CRLF
#endif

};

bool usb_stdio_init(void)
{
    if (get_core_num() != alarm_pool_core_num(alarm_pool_get_default()))
    {
        assert(false);
    }
    
#if !PICO_NO_BI_STDIO_USB
    bi_decl_if_func_used(bi_program_feature("USB stdin / stdout"));
#endif

    tusb_init();
    mutex_init(&stdio_usb_mutex);
    bool rc = true;
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
        rc = add_alarm_in_us(USB_STDIO_TASK_INTERVAL_US, timer_task, NULL, true) >= 0;
        memset(&one_shot_timer_crit_sec, 0, sizeof(one_shot_timer_crit_sec));
    }
    
    if (rc)
    {
        stdio_set_driver_enabled(&stdio_usb, true);
    }
    return rc;
}

bool usb_stdio_connected(void)
{
#if USB_STDIO_USB_CONNECTION_WITHOUT_DTR
    return tud_ready();
#else
    return tud_cdc_connected();
#endif
}

#else
#warning stdio USB was configured along with user use of TinyUSB device mode, but CDC is not enabled
bool stdio_usb_init(void) {
    return false;
}
#endif // CFG_TUD_ENABLED && CFG_TUD_CDC
