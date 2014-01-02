#ifndef ULTRASONIC_DRIVER_H
#define ULTRASONIC_DRIVER_H

#include "../include/protocol_rpi.h"
#include <pthread.h>
#include <stdio.h>

#define RX_TIME_LIMIT 3 //seconds
#define SOUND_WAVE_SPEED 343.5 // ms/s
enum RANGER_ERROR_CODES_ENUM { ERROR_CODE_TIME_LIMIT = 1 };

class US_ranger{
private:
    static RPiGPIOPin rs;
    static RPiGPIOPin te;

    static void* get_rx_signal(void *arg);

public:
    US_ranger( RPiGPIOPin rs_pin, RPiGPIOPin te_pin );
    // ~US_ranger();
    float get_distance();
};

#endif //ULTRASONIC_DRIVER_H