#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include <hardware/i2c.h>

#include "is3741.h"

#define SPARKLE_VERSION_MAJOR 0
#define SPARKLE_VERSION_MINOR 1
#define SPARKLE_VERSION_PATCH 0

#define SPARKLE_I2C_INSTANCE   i2c1
#define SPARKLE_GPIO_SLEEP  0
#define SPARKLE_GPIO_DIP1  25
#define SPARKLE_GPIO_SDA   26
#define SPARKLE_GPIO_SCL   27
#define SPARKLE_GPIO_INTB  28
#define SPARKLE_GPIO_SDB   29

#define SPARKLE_PANIC_SLEEP_INTERVAL_MS 100

typedef struct sparkle_context
{
    uint32_t i2c_baudrate;
    is3741_state_t* is3741;
    char command_buffer[256];
} sparkle_context_t;

sparkle_context_t* sparkle_init(void);
_Noreturn void sparkle_main(sparkle_context_t* sparkle);
void sparkle_exit(sparkle_context_t* sparkle);

#endif //BOARD_H