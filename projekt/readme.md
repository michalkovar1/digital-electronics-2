### Členové týmu

* Lukáš Kříž 
* Michal Kovář
* Jakub Křivánek

## Teoretický popis a vysvětlení

Tento kód představuje jednoduchou implementaci hry ve stylu Pong pro AVR mikrokontrolér s OLED displejem. Cílem hry je ovládat pálku tak, aby hráč co nejvíckrát odrážel padající míček a dosáhl největšího skóre.

## Instrukce
https://github.com/michalkovar1/digital-electronics-2/assets/124684834/de4a36de-1408-43fd-a312-3caa2765ab5b

## Popis hardweru demo aplikace
Schéma zapojení

![406278629_379350461132029_7878879748548183024_n](https://github.com/michalkovar1/digital-electronics-2/assets/124684834/bca8e501-57d1-4cd7-81e8-d39ac0fdf56e)


Reálné zapojení
![403751706_238876782422355_8697197447976309203_n](https://github.com/michalkovar1/digital-electronics-2/assets/124684834/9ac14f76-219f-4626-9c58-d8497a857fa3)

V zapojení jsou dvě tlačítka na ovládání pálky. Tlačítka josu zapojeny tak, aby reagovaly na vysokou úroveň. 
## Popis softwaru

První část kódu obsahuje inicializaci OLED displeje a definici proměnných pro ukládání důležitých parametrů hry a funkcí:

```c
// Definice pro OLED displej
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

oled_init(OLED_DISP_ON);
oled_clrscr();
int paddlePosition = 0;
int ballX = OLED_WIDTH / 2;
int ballY = OLED_HEIGHT - 60;
int ballSpeedX = 2;
int ballSpeedY = 2;
int score = 0;
```

### Hlavní smyčka
Hlavní smyčka hry neustále aktualizuje stav hry. Klíčové kroky zahrnují:

#### 1. Ovládání pálky + tlačítko RESET 
Nastavuje pozici pálky na základě stisknutých tlačítek (RIGHT_BUTTON a LEFT_BUTTON).
Zajišťuje, aby se pálka pohybovala v definovaných mezích.
Tlačítko RESET nám vrátí míček a pálku na původní hodnoty.

```c
        if ((PIND & (1 << RIGHT_BUTTON)) == 0 && paddlePosition < OLED_WIDTH - PADDLE_WIDTH) {
            paddlePosition += 6;
        }
        if ((PIND & (1 << LEFT_BUTTON)) == 0 && paddlePosition > 0) {
            paddlePosition -= 6;
        }

        if (((PIND & (1 << RESET_BUTTON)) == 0) {
            resetGame(&paddlePosition, &ballX, &ballY, &ballSpeedX, &ballSpeedY, &score);
        }
```

#### 2. Aktualizace míčku 
Mění pozici míčku na základě jeho aktuální rychlosti.
Řeší kolize s okraji displeje, což ovlivňuje trajektorii míčku.
```c
        // Aktualizace pozice míčku
        ballX += ballSpeedX;
        ballY += ballSpeedY;

  // Detekce kolizí s okraji displeje
        if (ballX <= 0 || ballX >= OLED_WIDTH - BALL_WIDTH) {
            ballSpeedX = -ballSpeedX;
        }

        if (ballY <= 0 || ballY >= OLED_HEIGHT - BALL_HEIGHT) {
            ballSpeedY = -ballSpeedY;
        }
```

#### 3. Detekce Kolize s Pálkou 
Rozpoznává kolize mezi míčkem a pálkou.
Mění směr míčku po kolizi a zvyšuje skóre.

```c
        // Detekce kolize s pálkou
        if (ballY + BALL_HEIGHT >= OLED_HEIGHT - PADDLE_HEIGHT - PADDLE_HEIGHT_MARGIN &&
            ballX + BALL_WIDTH >= paddlePosition &&
            ballX <= paddlePosition + PADDLE_WIDTH) {
            ballSpeedY = -ballSpeedY;
            score++;
        }

```

#### 4. Kreslení a Zobrazení: 
Mazání předchozích pozic pálky a míčku na displeji.
Vykreslení aktualizovaných pozic pálky a míčku.
Zobrazuje aktuální skóre na OLED displeji.

```c
oled_clear_screen();
drawPaddle(paddlePosition);
drawBall(ballX, ballY);
updateScore(score);
oled_display();
```

Dvě funkce usnadňují kreslení na OLED displeji:
1. drawPaddle():
Vykresluje pálku na displeji na základě její pozice.
Využívá funkci oled_drawLine.

2. drawball()
Zobrazuje míček na displeji podle jeho pozice.
Používá funkci oled_drawCircle.

#### 5. Funkce pro Aktualizaci Skóre:
Funkce updateScore spravuje aktualizaci a zobrazení aktuálního skóre v pravém horním rohu displeje. Pomocí funkce itoa, dojde k převedení intergeru na string.

```c
void updateScore(int score) {
    char scoreStr[5];  
    itoa(score, scoreStr, 10);
    oled_gotoxy(1, 0);
    oled_puts(scoreStr);
}
```c

#### 6. Funkce pro RESET hry:
Funkce resetGame() spravuje obnovení hodnot pálky a míčku, jako tomu bylo na začátku. Je nutné zde použít ukazatele. 
```c
void resetGame(int* paddlePosition, int* ballX, int* ballY, int* ballSpeedX, int* ballSpeedY, int* score) {    
    *paddlePosition = 0;
    *ballX = OLED_WIDTH / 2;  
    *ballY = OLED_HEIGHT / 2;  
    *ballSpeedX = 2;  
    *ballSpeedY = 2;  
    *score = 0;
        }
```
