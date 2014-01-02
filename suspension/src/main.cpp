#include "../drivers/ultrasonic_driver.h"
#include "../include/util.h"

using namespace std;

int main()
{
    init();

    US_ranger ranger( RPI_V2_GPIO_P1_11 );

    float dist = ranger.get_distance();
    printf( "distance is %f m\n", dist );

    // bcm2835_gpio_fsel( RPI_V2_GPIO_P1_11, BCM2835_GPIO_FSEL_OUTP );
    // bcm2835_gpio_fsel( RPI_V2_GPIO_P1_11, BCM2835_GPIO_FSEL_INPT );
    // bcm2835_gpio_set_pud( RPI_V2_GPIO_P1_11, BCM2835_GPIO_PUD_DOWN );

    // if ( HIGH == bcm2835_gpio_lev( RPI_V2_GPIO_P1_11 ) )
    //     printf("level HIGH\n");
    // else
    //     printf("level LOW\n");

    bcm2835_close();
    return 0;
}

