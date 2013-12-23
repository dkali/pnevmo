#include "../include/util.h"

using namespace std;

int init()
{
    if (!bcm2835_init())
        return 1;

    pin_init();
    
    return 0;
}