// https://github.com/Sylaina/oled-display
#define Button1 
#define Button2 

#include <oled.h>
#include "timer.h"
    uint8_t Player1; 
    uint8_t Player2;
    uint8_t Ball;
    uint8_t P_x1=5;
    uint8_t P_y1=20;
    uint8_t P_x2=5;
    uint8_t P_y2=44;

    uint8_t P2_x1=123;
    uint8_t P2_y1=20;
    uint8_t P2_x2=123;
    uint8_t P2_y2=44;

int main(void)
{
    TIM2_OVF_16US
    TIM2_OVF_ENABLE
    oled_init(OLED_DISP_ON);
    oled_clrscr();
    oled_charMode(NORMALSIZE);
    //oled_puts("128x64, SHH1106");

    Player1= oled_drawLine(P_x1, P_y1, P_x2, P_y2, WHITE);
    Player2= oled_drawLine(P2_x1, P2_y1, P2_x2, P2_y2, WHITE);
    Ball=oled_drawCircle(64,32,2,WHITE);
    void loop(){
        
        Player1 ++;
        Player2 ++;

        
    };

    //oled_gotoxy(0, 4);
    //oled_puts("BPC-DE2, Brno");

    // Copy buffer to display RAM
    oled_display();

    while (1) {
        ;
    }

    return 0;
}