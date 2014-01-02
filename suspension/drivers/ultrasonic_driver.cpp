#include "ultrasonic_driver.h"

RPiGPIOPin US_ranger::rs;
RPiGPIOPin US_ranger::te;

US_ranger::US_ranger( RPiGPIOPin rs_pin, RPiGPIOPin te_pin )
{
    te = te_pin;
    bcm2835_gpio_fsel( te, BCM2835_GPIO_FSEL_OUTP );
    bcm2835_gpio_write( te, LOW );

    rs = rs_pin;
    bcm2835_gpio_fsel( rs, BCM2835_GPIO_FSEL_INPT );
}

float US_ranger::get_distance()
{
    uint32_t t_start, t_end = 0;
    float distance = 0; // in meter
    pthread_t thRX;

    // TODO: add mutex here
    if ( HIGH == bcm2835_gpio_lev( rs ) )
    {
        printf("ERROR: initial level HIGH detected\n");
    }

    // start thread to receive signal
    // pthread_create( &thRX, NULL, &US_ranger::get_rx_signal, &t_end );

    //start measurement
    t_start = bcm2835_st_read();
    bcm2835_gpio_write( te, HIGH );
    bcm2835_delay( 250 );
    bcm2835_gpio_write( te, LOW );

    {
        uint32_t t_limit = 0;

        while(1)
        {
            int co = 0;
            if ( HIGH == bcm2835_gpio_lev( rs ) )
            {
                printf("level HIGH detected on %d try\n", co);
                t_end = bcm2835_st_read();
                break;
            }

            t_limit = bcm2835_st_read();
            if ( (t_limit - t_start)/1000000.0 > RX_TIME_LIMIT )
            {
                printf("ERROR: time limit exceeds in get_rx_signal, exiting function\n");
                break;
            }

            co++;
        }
    }

    // pthread_join(thRX, NULL);

    // calculate result
    if ( t_end != 0 )
    {
        printf("sta time %d \n", t_start);
        printf("end time %d \n", t_end);
        printf("diff: %f microsec\n", (t_end - t_start)/1000000.0 );
        distance = ((t_end - t_start)/1000000.0)*SOUND_WAVE_SPEED/2;
    }

    return distance;
}

void* US_ranger::get_rx_signal(void *arg)
{
    uint32_t t_enter, t_limit = 0;
    uint32_t *endtime = static_cast<uint32_t *>(arg);

    t_enter = bcm2835_st_read();
    // bcm2835_gpio_aren( rs );

    while(1)
    {
        int co = 0;
        // if ( HIGH == bcm2835_gpio_eds( rs ) )
        if ( HIGH == bcm2835_gpio_lev( rs ) )
        {
            printf("level HIGH detected on %d try\n", co);
            *endtime = bcm2835_st_read();
            break;
        }

        // if ( HIGH == bcm2835_gpio_lev( rs ) )
        // {
        //     printf("level HIGH detected\n");
        // }

        t_limit = bcm2835_st_read();
        if ( (t_limit - t_enter)/1000000.0 > RX_TIME_LIMIT )
        {
            printf("ERROR: time limit exceeds in get_rx_signal, exiting function\n");
            // bcm2835_gpio_clr_aren( rs );
            pthread_exit((void*)ERROR_CODE_TIME_LIMIT);
        }

        co++;
    }
    
    // bcm2835_gpio_set_eds(rs);
    // bcm2835_gpio_clr_aren(rs);
    return NULL;
}