#include "mt12232b_driver.h"

using namespace std;

MT12232B::MT12232B()
: lcd_pins {A0, E1, E2, RDWR, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7, RES },
  lcd_databus_pins {DB7, DB6, DB5, DB4, DB3, DB2, DB1, DB0}
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
}

void MT12232B::init_RES( RPiGPIOPin pin ){ RES = pin; }

void MT12232B::turn_on()
{
    clear_all_pins();
    bcm2835_delayMicroseconds( 20 );
    bcm2835_gpio_write( RES, HIGH );
    bcm2835_delayMicroseconds(5);

    cmd_reset();
    cmd_rmw(); //ReadModifyWrite off
    cmd_static_drive(false); //Включить обычный режим
    cmd_duty_select();//Мультиплекс 1/32
    cmd_display_start_line(0);//Верхнюю строку на 0
    cmd_adc_select(0);//NonInvert scan RAM
    cmd_display_onoff(true);//Display on

}

void MT12232B::rutn_off()
{

}

void MT12232B::clear_all_pins()
{
    for (int index = 0; index <LCD_PIN_NUM; index++){
        bcm2835_gpio_fsel(lcd_pins[index], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(lcd_pins[index], LOW);
    }
}

//Процедура выдачи байта в индикатор
void MT12232B::write_byte(int byte, int cd, int l, int r) {
//При необходимости настроить здесь шину данных на вывод
    bcm2835_gpio_write(RDWR, LOW);

    if (cd == 0)
        bcm2835_gpio_write(A0, LOW); //Выдача байта в индикатор как команды
    if (cd == 1)
        bcm2835_gpio_write(A0, HIGH); //Выдача байта в индикатор как данных

    write_to_bus(byte);        //Выдать байт на шину данных индикатора
    bcm2835_delayMicroseconds(50); //Это время предустановки адреса (tAW)
    
    //Выдать нужные стробы в индикатор
    if (l == 0)
        bcm2835_gpio_write(E1, LOW);
    if (l == 1)
        bcm2835_gpio_write(E1, HIGH);
    if (r == 0)
        bcm2835_gpio_write(E2, LOW);
    if (r == 1)
        bcm2835_gpio_write(E2, HIGH);
    bcm2835_delayMicroseconds(170); //Длительность сигналов E1,E2=1 (время предустановки данных попало сюда (tDS))
    
    //Сбросить сигналы E1 и E2
    bcm2835_gpio_write(E1, LOW);
    bcm2835_gpio_write(E2, LOW);
    bcm2835_delayMicroseconds(2000-40-160+10); //Минимально допустимый интервал между сигналами E1,E2=1
}

void MT12232B::write_to_bus(int byte)
{
    string bincmd = int_to_binstr(byte);
    printf( "write %s to bus\n", bincmd.c_str() );

    for ( int i =0; i < bincmd.length(); i++ )
    {
        char cvar = bincmd[i];
        int bit = stoi(string(1,cvar), NULL);
        // printf("bit %d\n", bit);

        RPiGPIOPin pin = lcd_databus_pins[i];
        if (bit == 0)
            bcm2835_gpio_write(pin, LOW);
        if (bit == 1)
            bcm2835_gpio_write(pin, HIGH);
    }
}


//======CONVERSION =========

string MT12232B::int_to_binstr(int byte)
{
    stringstream ss;
    ss << byte;

    unsigned n;
    ss >> n;
    bitset<8> b(n);
    return b.to_string();
}

//========COMMANDS==========

void MT12232B::cmd_reset()
{
    printf("cmd RESET\n");
    write_byte(0xE2, 0, 1, 1); //Команды выдаются всегда в оба кристалла индикатора
    clear_all_pins();
}

void MT12232B::cmd_rmw()
{
    printf("cmd RMW\n");
    write_byte(0xE0, 0, 1, 1);
    clear_all_pins();
}

void MT12232B::cmd_static_drive(bool bEnable)
{
    if (bEnable)
    {
        printf("cmd STATIC DRIVE ON\n");
        write_byte(0xA5, 0, 1, 1);
    }
    else
    {
        printf("cmd STATIC DRIVE OFF\n");
        write_byte(0xA4, 0, 1, 1);
    }
    clear_all_pins();
}

void MT12232B::cmd_duty_select()
{
    printf("cmd DUTY SELECT\n");
    write_byte(0xA9, 0, 1, 1);
    clear_all_pins();
}

// linenum 0...31
void MT12232B::cmd_display_start_line(int linenum)
{
    if (linenum < 0 || linenum > 31 )
    {
        printf("ERROR: cmd_display_start_line have invalid argument: %d", linenum);
        return;
    }

    printf("cmd DISPLAY START LINE: %d\n", linenum);

    bitset<8> b(linenum);
    b.set(6, 1);
    b.set(7, 1);
    // printf("%s\n", b.to_string().c_str());

    stringstream ss;
    ss << "0x" << hex << uppercase << b.to_ulong();

    string hexStr(ss.str());
    // printf("hex str: %s\n", hexStr.c_str());

    int iCmd;
    ss >> iCmd;
    write_byte(iCmd, 0, 1, 1);
    clear_all_pins();
}

// direction 0 - direct correspondence
// direction 1 - inverse correspondence
void MT12232B::cmd_adc_select(int direction)
{
    if (direction != 1 && direction != 0)
    {
        printf("ERROR: cmd_adc_select invalid argument: %d", direction);
        return;
    }

    printf("cmd ADC SELECT %d\n", direction);

    bitset<8> b(direction);
    b.set(5, 1);
    b.set(7, 1);
    // printf("%s\n", b.to_string().c_str());

    stringstream ss;
    ss << "0x" << hex << uppercase << b.to_ulong();

    string hexStr(ss.str());
    // printf("hex str: %s\n", hexStr.c_str());

    int iCmd;
    ss >> iCmd;
    write_byte(iCmd, 0, 1, 1);
    clear_all_pins();
}


//on true - display on
//on false - display off
void MT12232B::cmd_display_onoff(bool on)
{
    if (on)
    {
        printf("cmd DISPLAY ON\n");
        write_byte(0xAF, 0, 1, 1);
        clear_all_pins();
    }
    else
    {
        printf("cmd DISPLAY OFF\n");
        write_byte(0xAE, 0, 1, 1);
        clear_all_pins();   
    }
}
