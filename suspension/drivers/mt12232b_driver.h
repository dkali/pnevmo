#ifndef MT12232B_DRIVER_H
#define MT12232B_DRIVER_H

#include "../include/protocol_rpi.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include <string>

using namespace std;

const int LCD_PIN_NUM = 13;

class MT12232B{
private:
    RPiGPIOPin A0; //Адресный сигнал - выбор между передачей данных и команд управления
    RPiGPIOPin E1; //Разрешение обращений к модулю (а также строб данных)(1 кристалл)
    RPiGPIOPin E2; //Разрешение обращений к модулю (а также строб данных)(2 кристалл)
    RPiGPIOPin RDWR; //Выбор режима записи или чтения
    RPiGPIOPin DB0; //Шина данных (младший бит)
    RPiGPIOPin DB1;
    RPiGPIOPin DB2;
    RPiGPIOPin DB3;
    RPiGPIOPin DB4;
    RPiGPIOPin DB5;
    RPiGPIOPin DB6;
    RPiGPIOPin DB7; //Шина данных (старший бит)
    RPiGPIOPin RES; //Сброс / Выбор типа интерфейса

    RPiGPIOPin lcd_pins[LCD_PIN_NUM];
    RPiGPIOPin lcd_databus_pins[8];
    
    void clear_all_pins();
    void write_byte(int byte, int cd, int l, int r); //Процедура выдачи байта в индикатор
    void write_to_bus(int byte);
    string int_to_binstr(int byte);

public:
    MT12232B();
    void init_A0( RPiGPIOPin pin );
    void init_E1( RPiGPIOPin pin );
    void init_E2( RPiGPIOPin pin );
    void init_RDWR( RPiGPIOPin pin );
    void init_databus( RPiGPIOPin pin0, RPiGPIOPin pin1, RPiGPIOPin pin2, RPiGPIOPin pin3, RPiGPIOPin pin4, RPiGPIOPin pin5, RPiGPIOPin pin6, RPiGPIOPin pin7 );
    void init_RES( RPiGPIOPin pin );

    void turn_on();
    void rutn_off();

    void cmd_reset();
    void cmd_rmw();
    void cmd_static_drive(bool bEnable);
    void cmd_duty_select();
    void cmd_display_start_line(int linenum);
    void cmd_adc_select(int direction);
    void cmd_display_onoff(bool on);
};

#endif //MT12232B_DRIVER_H