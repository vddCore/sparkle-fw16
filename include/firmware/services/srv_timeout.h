#ifndef SRV_TIMEOUT_H
#define SRV_TIMEOUT_H

#include "firmware/kernel.h"

#define SRV_TIMEOUT_NOTIFICATION_MAX 4

typedef void (*srv_timeout_event_t)(kernel_context_t* kernel);

void srv_timeout_start(kernel_context_t* kernel);
void srv_timeout_stop(kernel_context_t* kernel);
void srv_timeout_task(kernel_context_t* kernel);
bool srv_timeout_is_running(void);

int8_t srv_timeout_add_event_handler(srv_timeout_event_t timeout_handler);
void srv_timeout_refresh(void);

#endif //SRV_TIMEOUT_H
