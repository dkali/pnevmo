#ifndef PROTOCOL_RPI_H
#define PROTOCOL_RPI_H

#include <bcm2835.h>

const int PIN_NUM = 18;

typedef RPiGPIOPin gpio_pin;

#define MODE_OUTPUT BCM2835_GPIO_FSEL_OUTP
#define MODE_INPUT BCM2835_GPIO_FSEL_INPT

static gpio_pin pins[PIN_NUM] = { RPI_V2_GPIO_P1_03,
                    RPI_V2_GPIO_P1_05,
                    RPI_V2_GPIO_P1_07,
                    RPI_V2_GPIO_P1_08,
                    RPI_V2_GPIO_P1_10,
                    RPI_V2_GPIO_P1_11,
                    RPI_V2_GPIO_P1_12,
                    RPI_V2_GPIO_P1_13,
                    RPI_V2_GPIO_P1_15,
                    RPI_V2_GPIO_P1_16,
                    RPI_V2_GPIO_P1_18,
                    RPI_V2_GPIO_P1_19,
                    RPI_V2_GPIO_P1_21,
                    RPI_V2_GPIO_P1_22,
                    RPI_V2_GPIO_P1_23,
                    RPI_V2_GPIO_P1_24,
                    RPI_V2_GPIO_P1_26 };

void pin_init();
void pin_mode( gpio_pin pin, int mode );
void pin_set( gpio_pin pin, int val );

#endif //PROTOCOL_RPI_H