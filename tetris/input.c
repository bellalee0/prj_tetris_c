#include "input.h"

#include <stdio.h>

#define ALPHABET_Q_UNDER 113
#define ALPHABET_Q_UPPER 81
#define ESC 27
#define SPACE_BAR 32

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>

#define ARROW 224
#define LEFT  75
#define RIGHT 77
#define UP    72

#else
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#define ARROW 91
#define LEFT 68
#define RIGHT 67
#define UP 65

#endif

GameKey GetKey(void) {

#ifdef _WIN32
    if (_kbhit()) {

        int key = _getch();

        if (key == ARROW) {
            key = _getch();

            switch (key) {
                case UP: return KEY_ROTATE;
                case LEFT: return KEY_LEFT;
                case RIGHT: return KEY_RIGHT;
            }
        }

        if (key == SPACE_BAR) {
            return KEY_HARD_DROP;
        }

        if (key == ESC || key == ALPHABET_Q_UNDER || key == ALPHABET_Q_UPPER) {
            return KEY_QUIT;
        }
    }
    return -1;

#else
    struct timeval tv = { 0, 0 };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(1, &fds, NULL, NULL, &tv) <= 0) return -1;

    int key = getchar();

    if (key == ESC) {

        if (select(1, &fds, NULL, NULL, &tv) <= 0) {
            return KEY_QUIT;
        }

        key = getchar();
        if (key == ARROW) {
            key = getchar();
            
            switch (key) {
                case UP: return KEY_ROTATE;
                case LEFT: return KEY_LEFT;
                case RIGHT: return KEY_RIGHT;
            }
        }
    }

    if (key == SPACE_BAR) {
        return KEY_HARD_DROP;
    }

    if (key == ALPHABET_Q_UNDER || key == ALPHABET_Q_UPPER) {
        return KEY_QUIT;
    }

#endif
}
