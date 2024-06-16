#ifndef SRV_SLEEP_H
#define SRV_SLEEP_H

void srv_sleep_exit(kernel_context_t* kernel);
void srv_sleep_enter(kernel_context_t* kernel, kernel_sleepstate_t state);
void srv_sleep_wake_on_command(kernel_context_t* kernel);
bool srv_sleep_is_sleeping(kernel_context_t* kernel);
void srv_sleep_idle_timeout_handler(kernel_context_t* kernel);
void srv_sleep_monitor_ec_task(kernel_context_t* kernel);

#endif //SRV_SLEEP_H
