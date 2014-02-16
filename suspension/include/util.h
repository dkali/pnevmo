#ifndef UTIL_H
#define UTIL_H

#include "../include/protocol_rpi.h"
#include "../drivers/ultrasonic_driver.h"
#include "../drivers/mt12232b_driver.h"
#include <cstdlib> //for rand()

int init();
void draw_melt_logo( MT12232B *display );
void scroll_ozu_up( MT12232B *display );
void scroll_ozu_down( MT12232B *display );
void draw_noize( MT12232B *display, int cycle );
void draw_noize_melt( MT12232B *display, int cycle );
void draw_black_screen( MT12232B *display );
void draw_alphabet( MT12232B *display );
void draw_distance( MT12232B *display, US_ranger *ranger );

#endif //UTIL_H