#include "../include/util.h"
#include <stdio.h>

using namespace std;

int init()
{
    if (!bcm2835_init())
        return 1;

    pin_init();
    
    return 0;
}


void draw_melt_logo( MT12232B *display ){
    for(int page=0; page<4; page++) { //Цикл по всем 4-м страницам индикатора
        display->cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
        display->cmd_set_address(0); //Установка текущего адреса для записи данных в 0
        for(int c=0; c<61; c++) { //Цикл вывода данных в левую половину индикатора
            display->write_byte( Logo122[page][c], 1, 1, 0); //Вывод очередного байта в индикатор
        }
        for(int c=61; c<122; c++) { //Цикл вывода данных в правую половину индикатора
           display->write_byte( Logo122[page][c], 1, 0, 1); //Вывод очередного байта в индикатор
        }
    }
}


void scroll_ozu_up( MT12232B *display ){
    for( int line_num = 0; line_num < 32; line_num++ )
    {
        display->cmd_display_start_line(line_num);
        bcm2835_delay(50);
    }
    display->cmd_display_start_line(0);
}


void scroll_ozu_down( MT12232B *display ){
    for( int line_num = 31; line_num >= 0; line_num-- )
    {
        display->cmd_display_start_line(line_num);
        bcm2835_delay(50);
    }
}


void draw_noize( MT12232B *display, int cycle ){
    int j = 0;
    while(j < cycle)
    {
        int page;
        for (page=0; page<4; page++)
        {
            display->cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
            display->cmd_set_address(0);

            int i;
            for (i=0;i<80;i++)
            {
                display->write_byte( rand() % 255, 1, 1, 1);
            }
        };
        j++;
    }
}


void draw_noize_melt( MT12232B *display, int cycle )
{
    int j = 0;
    while(j<cycle)
    {
        draw_noize( display, 1 );
        bcm2835_delay(20);

        draw_melt_logo( display );
        bcm2835_delay(10);

        j++;
    }
}


void draw_black_screen( MT12232B *display ){
    int page;
    for (page=0; page<4; page++)
    {
        display->cmd_set_page(page); //Установка текущей страницы для обоих кристаллов индикатора
        display->cmd_set_address(0);

        int i;
        for (i=0;i<61;i++)
        {
            display->write_byte( 0xFF, 1, 1, 1);
        }
    };
}


// TODO: rework with draw_text
void draw_alphabet( MT12232B *display ){
    display->cmd_set_page(0);
    display->cmd_set_address(0);
    
    unsigned char xChar;
    for (xChar=' '; xChar<(' ')+10; xChar++) display->print_char(xChar, 1, 0);
    for (xChar=' '+10; xChar<(' ')+20; xChar++) display->print_char(xChar, 0, 1);

    display->cmd_set_page(1);
    display->cmd_set_address(0);
    
    for (xChar=' '+20; xChar<(' ')+30; xChar++) display->print_char(xChar, 1, 0);
    for (xChar=' '+30; xChar<(' ')+40; xChar++) display->print_char(xChar, 0, 1);

    display->cmd_set_page(2);
    display->cmd_set_address(0);

    for (xChar=' '+40; xChar<(' ')+50; xChar++) display->print_char(xChar, 1, 0);
    for (xChar=' '+50; xChar<(' ')+60; xChar++) display->print_char(xChar, 0, 1);

    display->cmd_set_page(3);
    display->cmd_set_address(0);

    for (xChar=' '+60; xChar<(' ')+70; xChar++) display->print_char(xChar, 1, 0);
    for (xChar=' '+70; xChar<(' ')+80; xChar++) display->print_char(xChar, 0, 1);
}


void draw_distance( MT12232B *display, US_ranger *ranger ){
    while (true)
    {
        float dist;
        do {
            dist = ranger->get_distance();
        } while (dist <= 0);
        display->cmd_set_page(0);
        display->cmd_set_address(0);
        char buffer[50];
        int ln;
        ln = sprintf(buffer, "%6.2f cm", dist*100);
        for (int ch=0; ch<ln; ch++)
        {
            display->print_char(buffer[ch], 1, 0); // TODO: rework with draw_text
        }

        display->clear_line(1);
        display->cmd_set_address(0);
        for (int gg=0; gg<dist*61; gg++) display->write_byte( 0xAA, 1, 1, 0); // TODO: rework with draw_text
    }
}