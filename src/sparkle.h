#ifndef SPARKLE_H
#define SPARKLE_H

#include <stdint.h>

#include <hardware/i2c.h>

#include "is3741.h"

#define SPARKLE_VERSION_MAJOR 0
#define SPARKLE_VERSION_MINOR 1
#define SPARKLE_VERSION_PATCH 0

#define SPARKLE_I2C_INSTANCE   i2c1

#define SPARKLE_PANIC_SLEEP_INTERVAL_MS 100

#define SPARKLE_STATE_FLAG_WAKE_ON_COMMAND 1 << 0
#define SPARKLE_STATE_FLAG_SLEEP_REASON_COMMAND 1 << 1
#define SPARKLE_STATE_FLAG_SLEEP_REASON_EC 1 << 2
#define SPARKLE_STATE_FLAG_IGNORE_SLEEP 1 << 3

typedef struct sparkle_context
{
    is3741_state_t* is3741;
    uint8_t state_flags;
} sparkle_context_t;

sparkle_context_t* sparkle_init(void);
_Noreturn void sparkle_main(sparkle_context_t* sparkle);
void sparkle_exit(sparkle_context_t* sparkle);

#endif // SPARKLE_H