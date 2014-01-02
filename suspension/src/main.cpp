#include "../drivers/ultrasonic_driver.h"
#include "../include/util.h"

using namespace std;

int main()
{
    init();

    US_ranger ranger( RPI_V2_GPIO_P1_10, RPI_V2_GPIO_P1_08 );

    float dist = ranger.get_distance();
    printf( "distance is %f m\n", dist );

    return 0;
}

