#include "../drivers/ultrasonic_driver.h"
#include "../drivers/mt12232b_driver.h"
#include "../include/util.h"
#include <cstdlib>

using namespace std;

int main()
{
    init();

    // US_ranger ranger( RPI_V2_GPIO_P1_11 );

    // float dist = ranger.get_distance();
    // printf( "distance is %f m\n", dist );

    // bcm2835_gpio_fsel( RPI_V2_GPIO_P1_11, BCM2835_GPIO_FSEL_OUTP );
    // bcm2835_gpio_fsel( RPI_V2_GPIO_P1_11, BCM2835_GPIO_FSEL_INPT );
    // bcm2835_gpio_set_pud( RPI_V2_GPIO_P1_11, BCM2835_GPIO_PUD_DOWN );

    // while ( LOW == bcm2835_gpio_lev(RPI_V2_GPIO_P1_07) )
    // {
    // }
    // printf("HIGH level detected\n");

    MT12232B display;
    display.init_A0( RPI_V2_GPIO_P1_26 );
    display.init_E1( RPI_V2_GPIO_P1_23 );
    display.init_E2( RPI_V2_GPIO_P1_24 );
    display.init_RDWR( RPI_V2_GPIO_P1_21 ); //now is on gnd
    display.init_databus( RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_19, RPI_V2_GPIO_P1_18, RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_16, RPI_V2_GPIO_P1_13, RPI_V2_GPIO_P1_12, RPI_V2_GPIO_P1_11 );
    display.init_RES( RPI_V2_GPIO_P1_08 );

    display.turn_on();

    // Отображаем лого МЕЛТа
    for(int p=0; p<4; p++) { //Цикл по всем 4-м страницам индикатора
        display.cmd_set_page(p); //Установка текущей страницы для обоих кристаллов индикатора
        display.cmd_set_address(0); //Установка текущего адреса для записи данных в 0
        for(int c=0; c<61; c++) { //Цикл вывода данных в левую половину индикатора
            display.write_byte( Logo122[p][c], 1, 1, 0); //Вывод очередного байта в индикатор
        }
        for(int c=61; c<122; c++) { //Цикл вывода данных в правую половину индикатора
           display.write_byte( Logo122[p][c], 1, 0, 1); //Вывод очередного байта в индикатор
        }
    }

    bcm2835_delay(3000);
   
    // Бежим влево
    int j = 0;
    while(j < 1000)
    {
        int page;
        for (page=0;page<4;page++)
        {
            display.cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
            display.cmd_set_address(0);

            int i;
            for (i=0;i<80;i++)
            {
                display.write_byte( 0xAA+i+j, 1, 1, 1);
            }
        };
        //bcm2835_delay(20);
        j++;
    }

    // display.write_byte( 0x55, 1, 1, 0);
    // display.write_byte( 0x0F, 1, 1, 0);

    // Отражаем слева направо
    display.cmd_adc_select(1);

    // Бежим вверх
    for( int k = 0; k < 32; k++ )
    {
        display.cmd_display_start_line(k);
        bcm2835_delay(50);
    }

    display.cmd_adc_select(0);

    // Бежим вправо
    j = 0;
    while(j < 10)
    {
        int page;
        for (page=0;page<4;page++)
        {
            display.cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
            display.cmd_set_address(0);

            int i;
            for (i=0;i<80;i++)
            {
                display.write_byte( 0xAA+i+j, 1, 1, 1);
            }
        };
        j++;
        bcm2835_delay(20);
    }

    // Шумим
    j = 0;
    while(j < 1000)
    {
        int page;
        for (page=0;page<4;page++)
        {
            display.cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
            display.cmd_set_address(0);

            int i;
            for (i=0;i<80;i++)
            {
                display.write_byte( rand() % 255, 1, 1, 1);
            }
        };
        j++;
    }

    // Ч0рный экран
    int page;
    for (page=0;page<4;page++)
    {
        display.cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
        display.cmd_set_address(0);

        int i;
        for (i=0;i<80;i++)
        {
            display.write_byte( 0xFF, 1, 1, 1);
        }
    };

    // Шумим Мелт
    j = 0;
    while(true)
    {
        int page;
        for (page=0;page<4;page++)
        {
            display.cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
            display.cmd_set_address(0);

            int i;
            for (i=0;i<80;i++)
            {
                display.write_byte( rand() % 255, 1, 1, 1);
            }
        };
        j++;

        bcm2835_delay(20);
        for(int p=0; p<4; p++) { //Цикл по всем 4-м страницам индикатора
            display.cmd_set_page(p); //Установка текущей страницы для обоих кристаллов индикатора
            display.cmd_set_address(0); //Установка текущего адреса для записи данных в 0
            for(int c=0; c<61; c++) { //Цикл вывода данных в левую половину индикатора
                display.write_byte( Logo122[p][c], 1, 1, 0); //Вывод очередного байта в индикатор
            }
            for(int c=61; c<122; c++) { //Цикл вывода данных в правую половину индикатора
               display.write_byte( Logo122[p][c], 1, 0, 1); //Вывод очередного байта в индикатор
            }
        }
        bcm2835_delay(10);
    }

    // display.cmd_display_onoff(false);

    bcm2835_close();
    return 0;
}

