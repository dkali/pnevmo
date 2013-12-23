#ifndef ULTRASONIC_DRIVER_H
#define ULTRASONIC_DRIVER_H

#include "../include/protocol_rpi.h"

class US_ranger{
private:
    gpio_pin rs;
    gpio_pin te;

public:
    US_ranger( gpio_pin rs_pin, gpio_pin te_pin );
    // ~US_ranger();
};

#endif //ULTRASONIC_DRIVER_H