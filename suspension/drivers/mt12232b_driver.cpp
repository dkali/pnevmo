#include "mt12232b_driver.h"

using namespace std;

MT12232B::MT12232B()
{}

void MT12232B::init_A0( RPiGPIOPin pin ){ A0 = pin; }

void MT12232B::init_E1( RPiGPIOPin pin ){ E1 = pin; }

void MT12232B::init_E2( RPiGPIOPin pin ){ E2 = pin; }

void MT12232B::init_RDWR( RPiGPIOPin pin ){ RDWR = pin; }

void MT12232B::init_databus( RPiGPIOPin pin0, RPiGPIOPin pin1, RPiGPIOPin pin2, RPiGPIOPin pin3, RPiGPIOPin pin4, RPiGPIOPin pin5, RPiGPIOPin pin6, RPiGPIOPin pin7 )
{
    DB0 = pin0;
    DB1 = pin1;
    DB2 = pin2;
    DB3 = pin3;
    DB4 = pin4;
    DB5 = pin5;
    DB6 = pin6;
    DB7 = pin7;

    lcd_databus_pins[0] = DB7;
    lcd_databus_pins[1] = DB6;
    lcd_databus_pins[2] = DB5;
    lcd_databus_pins[3] = DB4;
    lcd_databus_pins[4] = DB3;
    lcd_databus_pins[5] = DB2;
    lcd_databus_pins[6] = DB1;
    lcd_databus_pins[7] = DB0;

    int i;
    lcd_pins[0] = A0;
    lcd_pins[1] = E1;
    lcd_pins[2] = E2;
    lcd_pins[3] = RDWR;
    for (i=0;i<8;i++) lcd_pins[i+4] = lcd_databus_pins[i];
    lcd_pins[12] = RES;
}

void MT12232B::init_RES( RPiGPIOPin pin ){ RES = pin; }

void MT12232B::turn_on()
{
    clear_all_pins();
    bcm2835_gpio_write( RES, LOW );
    bcm2835_delayMicroseconds( 10 );
    bcm2835_gpio_write( RES, HIGH );
    bcm2835_delay(4);

    // cmd_reset();
    cmd_rmw_end(); //ReadModifyWrite off
    cmd_static_drive(false); //Включить обычный режим
    cmd_duty_select();//Мультиплекс 1/32
    cmd_display_onoff(true);//Display on
    cmd_display_start_line(0);//Верхнюю строку на 0
    cmd_adc_select(0);//NonInvert scan RAM

}

void MT12232B::clear_all_pins()
{
    for (int index = 0; index <LCD_PIN_NUM-1; index++){
        bcm2835_gpio_fsel(lcd_pins[index], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(lcd_pins[index], LOW);
    }
}

//Процедура выдачи байта в индикатор
void MT12232B::write_byte(int byte, int cd, int l, int r) {
//При необходимости настроить здесь шину данных на вывод
    bcm2835_gpio_write(RDWR, LOW);

    int val = 0;
    val = cd ? HIGH : LOW;
    bcm2835_gpio_write(A0, val); //Выдача байта в индикатор как команды (0) или данных (1)
    bcm2835_delayMicroseconds(40*0); //Taw

    //Выдать нужные стробы в индикатор
    val = l ? HIGH : LOW;
    bcm2835_gpio_write(E1, val);

    val = r ? HIGH : LOW;
    bcm2835_gpio_write(E2, val);

    write_to_bus(byte);        //Выдать байт на шину данных индикатора
    bcm2835_delayMicroseconds(160*0); //Tds
    
    //Сбросить сигналы E1 и E2
    bcm2835_gpio_write(E1, LOW);
    bcm2835_gpio_write(E2, LOW);
    bcm2835_delayMicroseconds(20*0); //Tdh
    
    bcm2835_delayMicroseconds(2); //Tcyc
}

void MT12232B::write_to_bus(int byte)
{
    int mask = 128;
    for ( int i = 0; i < 8; i++ )
    {
        int bit = (byte & mask) >> (7-i);
        RPiGPIOPin pin = lcd_databus_pins[i];
        bcm2835_gpio_write(pin, bit);
        mask >>= 1;
    }
}


//========COMMANDS==========

void MT12232B::cmd_reset()
{
    // printf("cmd RESET\n");
    write_byte(0xE2, 0, 1, 1); //Команды выдаются всегда в оба кристалла индикатора
    clear_all_pins();
}


void MT12232B::cmd_rmw()
{
    // printf("cmd RMW\n");
    write_byte(0xE0, 0, 1, 1);
    clear_all_pins();
}


void MT12232B::cmd_rmw_end()
{
    // printf("cmd RMW END\n");
    write_byte(0xEE, 0, 1, 1);
    clear_all_pins();
}


void MT12232B::cmd_static_drive(bool bEnable)
{
    if (bEnable)
    {
        // printf("cmd STATIC DRIVE ON\n");
        write_byte(0xA5, 0, 1, 1);
    }
    else
    {
        // printf("cmd STATIC DRIVE OFF\n");
        write_byte(0xA4, 0, 1, 1);
    }
    clear_all_pins();
}


void MT12232B::cmd_duty_select()
{
    // printf("cmd DUTY SELECT\n");
    write_byte(0xA9, 0, 1, 1);
    clear_all_pins();
}


// line_num 0...31
void MT12232B::cmd_display_start_line(unsigned int line_num)
{
    if ( line_num > 31 )
    {
        printf("ERROR: cmd_display_start_line have invalid argument: %d\n", line_num);
        return;
    }

    int iCmd;
    iCmd = line_num | (1 << 6) | (1 << 7);
    write_byte(iCmd, 0, 1, 1);
    clear_all_pins();
}


// direction 0 - direct correspondence
// direction 1 - inverse correspondence
void MT12232B::cmd_adc_select(int direction)
{
    if (direction != 1 && direction != 0)
    {
        printf("ERROR: cmd_adc_select invalid argument: %d\n", direction);
        return;
    }

    int iCmd;
    iCmd = direction | ( 1 << 5) | ( 1 << 7 );
    write_byte(iCmd, 0, 1, 1);
    clear_all_pins();
}


//on true - display on
//on false - display off
void MT12232B::cmd_display_onoff(bool on)
{
    if (on)
        printf("cmd DISPLAY ON\n");
    else
        printf("cmd DISPLAY OFF\n");
    write_byte(on ? 0xAF : 0xAE, 0, 1, 1);
    clear_all_pins();
}


// pnum 0...3, page number
void MT12232B::cmd_set_page(int pnum)
{
    if (pnum <0 && pnum >3)
    {
        printf("ERROR: cmd_set_page bad argument: %d\n", pnum);
        return;
    }

    // printf("cmd SET PAGE %d\n", pnum);
    write_byte(pnum|0xB8, 0, 1, 1); //Установка текущей страницы для обоих кристаллов индикатора
    clear_all_pins();
}


// column_address 0...79
void MT12232B::cmd_set_address(int column_address)
{
    if (column_address <0 && column_address >79)
    {
        printf("ERROR: cmd_set_address bad argument: %d\n", column_address);
        return;
    }

    // printf("cmd SET ADDRESS: %d\n", column_address);
    
    int iCmd;
    iCmd = column_address;
    write_byte(iCmd, 0, 1, 1);
    clear_all_pins();
}


// ============== Print text ================

void MT12232B::print_char(unsigned char xChar, int l, int r){
    unsigned char symbol[8];

    for (int i=0; i<8; i++) symbol[i] = fontdata_8x8[xChar*8 + i];

    int col, row;
    for (col=0; col<8; col++)
    {
        int xByte = 0;
        for (row=0; row<7; row++) xByte |= ((symbol[row] >> (7-col)) & 1) << (row); //rotate symbol. Trust me.
        write_byte(xByte, 1, l, r);
    }
}


void MT12232B::print_text( const char *xStr, unsigned int line_num){
    unsigned char symbol[8];

    if (line_num > 3 ){
        printf("ERROR: print_text has incorrect line_num: %d\n", line_num);
        return;
    }

    clear_line(line_num);
    cmd_set_address(0);

    int bits_written = 0;
    int len = strlen(xStr);
    printf("len: %d\n", len);
    for (int ch=0; ch<len; ch++)
    {
        printf("char: %c\n", xStr[ch]);
        // if (ch == 62) cmd_set_address(0);
        print_char(xStr[ch], ch>61?0:1, ch>61?1:0);
        bits_written += 8;
    }
}


void MT12232B::clear_screen(){
    int page;
    for (page=0; page<4; page++)
    {
        cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
        cmd_set_address(0);

        int i;
        for (i=0; i<61; i++)
        {
            write_byte( 0x00, 1, 1, 1);
        }
    };
}


void MT12232B::clear_line( unsigned int line_num ){
    if ( line_num > 3 )
    {
        printf("ERROR: clear_line has bad line_num: %d\n", line_num);
        return;
    }

    cmd_set_page(line_num);
    cmd_set_address(0);

    int i;
    for (i=0; i<61; i++)
    {
        write_byte( 0x00, 1, 1, 1);
    }
}