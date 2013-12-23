#include "../drivers/ultrasonic_driver.h"
#include "../include/util.h"

using namespace std;

int main()
{
    init();

    US_ranger ranger( RPI_V2_GPIO_P1_05, RPI_V2_GPIO_P1_07 );
    
    // pin_mode( RPI_V2_GPIO_P1_03, BCM2835_GPIO_FSEL_OUTP );
    // pin_set( RPI_V2_GPIO_P1_03, HIGH );

    return 0;
}

