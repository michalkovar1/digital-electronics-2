#include <avr/io.h>
#include <avr/interrupt.h>
#include "oled.h"

// Definice pro parametry OLED displeje
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Vlastnosti pálky
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 5
#define PADDLE_HEIGHT_MARGIN 1

// Vlastnosti míčku
#define BALL_WIDTH 5
#define BALL_HEIGHT 5

// Piny pro tlačítka
#define RIGHT_BUTTON PD7
#define LEFT_BUTTON PD5
#define RESET_BUTTON PD3

// Deklarace funkcí
void oled_clear_screen(void);
void drawPaddle(int position);
void drawBall(int x, int y);
void updateScore(int score);
void drawGameOver(void);
void resetGame(int* paddlePosition, int* ballX, int* ballY, int* ballSpeedX, int* ballSpeedY, int* score);
int main(void) {
    oled_init(OLED_DISP_ON);
    oled_clrscr();
    int paddlePosition = 0;
    int ballX = OLED_WIDTH/2 ;
    int ballY = OLED_HEIGHT-60;
    int ballSpeedX = 2;
    int ballSpeedY = 2;
    int score = 0;

    // Hlavní smyčka programu
    while (1) {
        // Aktualizace pozice pálky na základě stisknutých tlačítek
        if ((PIND & (1 << RIGHT_BUTTON)) == 0 && paddlePosition < OLED_WIDTH - PADDLE_WIDTH) {
            paddlePosition += 6;
        }
        if ((PIND & (1 << LEFT_BUTTON)) == 0 && paddlePosition > 0) {
            paddlePosition -= 6;
        }

        // Aktualizace pozice míčku
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Detekce kolizí s okraji displeje
        if (ballX <= 0 || ballX >= OLED_WIDTH - BALL_WIDTH) {
            ballSpeedX = -ballSpeedX;
        }

        if (ballY-OLED_HEIGHT <= 0) {
            ballSpeedY = -ballSpeedY;
        }

        // Detekce kolize s pálkou
        if (ballY + BALL_HEIGHT >= OLED_HEIGHT - PADDLE_HEIGHT - PADDLE_HEIGHT_MARGIN &&
            ballX + BALL_WIDTH >= paddlePosition &&
            ballX <= paddlePosition + PADDLE_WIDTH) {
            ballSpeedY = -ballSpeedY;
            score++;
        }

        // Vymazání staré pozice pálky a vykreslení nové
        oled_clear_screen();
        drawPaddle(paddlePosition);
        drawBall(ballX, ballY);
        updateScore(score);
        oled_display();

        if(ballY + BALL_HEIGHT >= OLED_HEIGHT){
            drawGameOver();
                while (((PIND & (1 << RESET_BUTTON))) == 0);
                    resetGame(&paddlePosition, &ballX, &ballY, &ballSpeedX, &ballSpeedY, &score);
        }

    }

    return 0;
}

void drawPaddle(int position) {
    // Funkce pro vykreslení pálky na displeji
    oled_drawLine(position, OLED_HEIGHT - PADDLE_HEIGHT, position + PADDLE_WIDTH, OLED_HEIGHT - PADDLE_HEIGHT, WHITE);
}

void drawBall(int x, int y) {
    // Funkce pro vykreslení míčku na displeji
    oled_drawCircle(x, y, BALL_WIDTH / 2, WHITE);
}

void updateScore(int score) {
    // Funkce pro zobrazení skóre v pravém horním rohu
    char scoreStr[5];
    itoa(score,scoreStr,10 );
    oled_gotoxy(1, 0);
    oled_puts(scoreStr);
}

void oled_clear_screen(void) {
    // Implementace mazání obrazovky OLED displeje
    oled_clear_buffer();
}

void drawGameOver(){
    //Funkce pro ukončení hry
    oled_clrscr();
    oled_charMode(DOUBLESIZE);
    oled_gotoxy(0, 5);
    oled_puts("KONEC");
    oled_display();
}
void resetGame(int* paddlePosition, int* ballX, int* ballY, int* ballSpeedX, int* ballSpeedY, int* score) {
    //Funkce pro reset hry
    *paddlePosition = 0;
    *ballX = OLED_WIDTH / 2;
    *ballY = OLED_HEIGHT - 60;
    *ballSpeedX = 2;
    *ballSpeedY = 2;
    *score = 0;
}
