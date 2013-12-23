#include "ultrasonic_driver.h"

US_ranger::US_ranger( gpio_pin rs_pin, gpio_pin te_pin )
{
    te = te_pin;
    pin_mode( te, MODE_OUTPUT );
    pin_set( te, LOW );
}
