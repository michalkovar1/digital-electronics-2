### Členové týmu

* Lukáš Kříž 
* Michal Kovář
* Jakub Křivánek

## Teoretický popis a vysvětlení

Tento kód představuje jednoduchou implementaci hry ve stylu Pong pro AVR mikrokontrolér s OLED displejem. Cílem hry je ovládat pálku tak, aby hráč co nejvíckrát odrážel padající míček a dosáhl největšího skóre.

## Instrukce

Write an instruction manual for your application, including photos and a link to a short app video.

## Popis hardweru demo aplikace

Insert descriptive text and schematic(s) of your implementation.

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

#### 1. Ovládání pálky 
Nastavuje pozici pálky na základě stisknutých tlačítek (RIGHT_BUTTON a LEFT_BUTTON).
Zajišťuje, aby se pálka pohybovala v definovaných mezích.

```c
        if ((PIND & (1 << RIGHT_BUTTON)) == 0 && paddlePosition < OLED_WIDTH - PADDLE_WIDTH) {
            paddlePosition += 6;
        }
        if ((PIND & (1 << LEFT_BUTTON)) == 0 && paddlePosition > 0) {
            paddlePosition -= 6;
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

Funkce pro Aktualizaci Skóre
Funkce updateScore spravuje aktualizaci a zobrazení aktuálního skóre v pravém horním rohu displeje. Pomocí funkce itoa, dojde k převedení intergeru na string.

```c
void updateScore(int score) {
    char scoreStr[5];  
    itoa(score, scoreStr, 10);
    oled_gotoxy(1, 0);
    oled_puts(scoreStr);
}
```
