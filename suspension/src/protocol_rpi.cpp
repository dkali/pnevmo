#include "../include/protocol_rpi.h"

using namespace std;

//initialize all GPIO pins as output with 0 v
void pin_init()
{
    for (int index = 0; index <PIN_NUM; index++){
        bcm2835_gpio_fsel(pins[index], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(pins[index], LOW);
    }
}

void pin_mode( gpio_pin pin, int mode)
{
    bcm2835_gpio_fsel(pin, mode);
}

void pin_set( gpio_pin pin, int val )
{
    bcm2835_gpio_write(pin, val);
}