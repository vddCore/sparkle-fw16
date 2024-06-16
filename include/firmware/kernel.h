#ifndef KERNEL_H
#define KERNEL_H

#include "drivers/is3741.h"

#define SPARKLE_VERSION_MAJOR 1
#define SPARKLE_VERSION_MINOR 0
#define SPARKLE_VERSION_PATCH 0

#define KERNEL_PANIC_INTERVAL_MS 100

#define KERNEL_STATEFLAG_SLEEPSTATE_MASK     0b00001110
#define KERNEL_STATEFLAG_WAKE_ON_COMMAND_MASK 0b00000001

typedef enum kernel_sleepstate
{
    KERNEL_SLEEPSTATE_AWAKE       = 0x00,
    KERNEL_SLEEPSTATE_EC_SLEEP    = 0x01,
    KERNEL_SLEEPSTATE_USB_SUSPEND = 0x02,
    KERNEL_SLEEPSTATE_COMMAND     = 0x03,
    KERNEL_SLEEPSTATE_TIMEOUT     = 0x04
} __attribute__((packed)) kernel_sleepstate_t;

typedef struct kernel_context
{
    is3741_state_t* is3741;
    int32_t sleep_timeout_ms;    /* -1: ignore all requests, 0: disable timeout */
    uint8_t state_flags;
} kernel_context_t;

kernel_context_t* kernel_init(void);
_Noreturn void kernel_main(void);
_Noreturn void kernel_panic(const char* why);

#endif // KERNEL_H
