#include <hardware/gpio.h>

#include "drivers/pins.h"

void pins_init(void)
{
    gpio_set_function(PIN_GPIO_SDB, GPIO_FUNC_SIO);
    gpio_set_outover(PIN_GPIO_SDB, GPIO_OVERRIDE_HIGH);
    gpio_set_oeover(PIN_GPIO_SDB, GPIO_OVERRIDE_HIGH);
    gpio_set_dir(PIN_GPIO_SDB, true);

    gpio_set_function(PIN_GPIO_SLEEP, GPIO_FUNC_SIO);
    gpio_set_dir(PIN_GPIO_SLEEP, false);
    gpio_pull_down(PIN_GPIO_SLEEP);

    gpio_set_function(PIN_GPIO_DIP1, GPIO_FUNC_SIO);
    gpio_set_dir(PIN_GPIO_DIP1, false);
    gpio_pull_down(PIN_GPIO_DIP1);

    gpio_set_function(PIN_GPIO_INTB, GPIO_FUNC_SIO);
    gpio_set_dir(PIN_GPIO_INTB, false);
    gpio_pull_down(PIN_GPIO_INTB);

    gpio_set_function(PIN_GPIO_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_GPIO_SDA);
    
    gpio_set_function(PIN_GPIO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_GPIO_SCL);
}

void pins_set_is3741_sdb(bool enable)
{
    gpio_put(PIN_GPIO_SDB, enable);
}

bool pins_get_sleep(void)
{
    return gpio_get(PIN_GPIO_SLEEP);
}

bool pins_get_dip1(void)
{
    return gpio_get(PIN_GPIO_DIP1);
}

bool pins_get_intb(void)
{
    return gpio_get(PIN_GPIO_INTB);
}