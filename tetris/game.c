#include "game.h"
#include "console.h"

#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ARROW 224
#define ENTER 13

int x = 30;
int y = 10;

#ifdef _WIN32

int GetKey() {
    if (_kbhit()) {
        return _getch();
    }
    return -1;
}

#else

int GetKey() {
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

#endif

void RunGame(void) {

    CursorView(0);
    GotoXY(x, y);

    while (1) {

        int nkey = GetKey();

        if (nkey == ENTER) {
            GotoXY(x, y);
            TextColor(GREEN);
            printf("■");
            fflush(stdout);
        }

#ifdef _WIN32

        if (nkey == ARROW) {

            nkey = _getch();

            switch (nkey) {
                case UP:
                    y--;
                    break;

                case DOWN:
                    y++;
                    break;

                case LEFT:
                    x--;
                    break;

                case RIGHT:
                    x++;
                    break;
            }
        }

#endif

        GotoXY(x, y);
    }
}