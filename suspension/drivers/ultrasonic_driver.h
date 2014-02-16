#ifndef ULTRASONIC_DRIVER_H
#define ULTRASONIC_DRIVER_H

#include "../include/protocol_rpi.h"
#include <stdio.h>

#define SIG_TIME_LIMIT 20 //miliseconds
#define SOUND_WAVE_SPEED 343.5 // ms/s
enum RANGER_ERROR_CODES_ENUM { ERROR_CODE_TIME_LIMIT = 1 };

class US_ranger{
private:
    RPiGPIOPin sig;

public:
    US_ranger( RPiGPIOPin sig_pin );
    float get_distance();
};

#endif //ULTRASONIC_DRIVER_H