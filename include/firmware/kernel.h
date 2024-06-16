#ifndef KERNEL_H
#define KERNEL_H

#include <hardware/i2c.h>

#include "drivers/is3741.h"

#define SPARKLE_VERSION_MAJOR 0
#define SPARKLE_VERSION_MINOR 1
#define SPARKLE_VERSION_PATCH 0

#define SPARKLE_I2C_INSTANCE i2c1

#define SPARKLE_PANIC_SLEEP_INTERVAL_MS 100

#define SPARKLE_STATEFLAG_SLEEP_IGNORE    0b00001000
#define SPARKLE_STATEFLAG_SLEEP_STATE     0b00000110
#define SPARKLE_STATEFLAG_WAKE_ON_COMMAND 0b00000001

typedef struct kernel_context
{
    is3741_state_t* is3741;
    uint8_t state_flags;
} kernel_context_t;

kernel_context_t* kernel_init(void);
_Noreturn void kernel_main(kernel_context_t* kernel);
_Noreturn void kernel_panic(const char* why);
void kernel_exit(kernel_context_t* kernel);

#endif // KERNEL_H
