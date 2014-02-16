#include "ultrasonic_driver.h"

US_ranger::US_ranger( RPiGPIOPin sig_pin )
{
    sig = sig_pin;
    bcm2835_gpio_fsel( sig, BCM2835_GPIO_FSEL_OUTP );
    // printf("pin = %d\n", sig_pin);
    bcm2835_gpio_write( sig, LOW );
}

float US_ranger::get_distance()
{
    uint32_t t_start = 0;
    uint32_t t_high = 0;
    uint32_t t_low = 0;
    uint32_t t_current = 0;
    float distance = 0; // in meter

    // TODO: add mutex here
    if ( HIGH == bcm2835_gpio_lev( sig ) )
    {
        printf("ERROR: initial level HIGH detected\n");
    }

    //send command
    bcm2835_gpio_fsel( sig, BCM2835_GPIO_FSEL_OUTP );
    bcm2835_gpio_write( sig, LOW );
    bcm2835_delayMicroseconds( 2 );

    bcm2835_gpio_write( sig, HIGH );
    bcm2835_delayMicroseconds( 5 );
    bcm2835_gpio_write( sig, LOW );

    // get response
    bcm2835_gpio_fsel( sig, BCM2835_GPIO_FSEL_INPT );
    bcm2835_gpio_set_pud( sig, BCM2835_GPIO_PUD_DOWN ); //this doesn't work
    t_start = bcm2835_st_read();
    
    int count = 0;
    while(1)
    {
        if ( HIGH == bcm2835_gpio_lev( sig ) )
        {
            // printf("level HIGH detected on %d try\n", count);
            t_high = bcm2835_st_read();

            while(1)
            {
                if ( LOW == bcm2835_gpio_lev( sig ) )
                {
                    t_low = bcm2835_st_read();
                    // printf("level LOW detected on %d try\n", count);
                    break;
                }

                t_current = bcm2835_st_read();
                if ( (t_current - t_start)/1000.0 > SIG_TIME_LIMIT )
                {
                    // printf("ERROR: low signal is not received\n");
                    break;
                }
                count++;
            }

            break;
        }

        t_current = bcm2835_st_read();
        if ( (t_current - t_start)/1000.0 > SIG_TIME_LIMIT )
        {
            // printf("ERROR: high signal is not received\n");
            break;
        }

        count++;
    }

    distance = -1; //default return value
    // calculate result
    if ( t_low != 0 )
    {
        // printf("sta time %u \n", t_high);
        // printf("end time %u \n", t_low);
        // printf("diff: %f sec\n", (t_low - t_high)/1000000.0 );
        distance = ((t_low - t_high)/1000000.0)*SOUND_WAVE_SPEED/2;
    }

    bcm2835_gpio_fsel( sig, BCM2835_GPIO_FSEL_OUTP );
    return distance;
}
