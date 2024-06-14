#ifndef PINS_H
#define PINS_H

#include <stdbool.h>

#define PIN_GPIO_SLEEP  0
#define PIN_GPIO_DIP1  25
#define PIN_GPIO_SDA   26
#define PIN_GPIO_SCL   27
#define PIN_GPIO_INTB  28
#define PIN_GPIO_SDB   29

void pins_init(void);
void pins_set_is3741_sdb(bool enable);
bool pins_get_sleep(void);
bool pins_get_dip1(void);
bool pins_get_intb(void);

#endif //PINS_H
