#include "../drivers/ultrasonic_driver.h"
#include "../drivers/mt12232b_driver.h"
#include "../include/util.h"


using namespace std;

int main()
{
    init();

    US_ranger ranger( RPI_V2_GPIO_P1_03 );

    MT12232B display;
    display.init_A0( RPI_V2_GPIO_P1_26 );
    display.init_E1( RPI_V2_GPIO_P1_23 );
    display.init_E2( RPI_V2_GPIO_P1_24 );
    display.init_RDWR( RPI_V2_GPIO_P1_21 ); //now is on gnd
    display.init_databus( RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_19, RPI_V2_GPIO_P1_18, RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_16, RPI_V2_GPIO_P1_13, RPI_V2_GPIO_P1_12, RPI_V2_GPIO_P1_11 );
    display.init_RES( RPI_V2_GPIO_P1_08 );

    display.turn_on();

    // draw_melt_logo(&display);
    // bcm2835_delay(3000);

    // scroll_ozu_up(&display);
    // bcm2835_delay(3000);

    // scroll_ozu_down(&display);
    // bcm2835_delay(3000);

    // draw_noize( &display, 1000 );
    // draw_noize_melt( &display, 100 );

    // draw_black_screen( &display );
    // bcm2835_delay(3000);

    display.clear_screen();
    // display.clear_line(2);

    // draw_alphabet( &display );
    // bcm2835_delay(3000);

    // draw_distance( &display, &ranger );

    display.print_text( "Hello world!", 1);



    // // Отражаем слева направо
    // display.cmd_adc_select(1);

    // display.cmd_adc_select(0);


    // // Бежим влево
    // int j = 0;
    // while(j < 1000)
    // {
    //     int page;
    //     for (page=0;page<4;page++)
    //     {
    //         display.cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
    //         display.cmd_set_address(0);

    //         int i;
    //         for (i=0;i<80;i++)
    //         {
    //             display.write_byte( 0xAA+i+j, 1, 1, 1);
    //         }
    //     };
    //     //bcm2835_delay(20);
    //     j++;
    // }


    // // Бежим вправо
    // j = 0;
    // while(j < 10)
    // {
    //     int page;
    //     for (page=0;page<4;page++)
    //     {
    //         display.cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
    //         display.cmd_set_address(0);

    //         int i;
    //         for (i=0;i<80;i++)
    //         {
    //             display.write_byte( 0xAA+i+j, 1, 1, 1);
    //         }
    //     };
    //     j++;
    //     bcm2835_delay(20);
    // }
    
    // display.cmd_display_onoff(false);

    bcm2835_close();
    return 0;
}

