#include "game.h"
#include "console.h"
#include "render.h"
#include "logic.h"
#include "input.h"

#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

int board[BOARD_ROWS][BOARD_COLS];

void RunGame(void) {

#ifndef _WIN32
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif

    srand(time(NULL));
    CursorView(0);
    DrawBorder();

    for (int i = 0; i < 10; i++) {
        ActiveBlock tetromino = SpawnTetromino();

        DrawTetromino(tetromino);

        GameKey key = GetKey();
        EraseTetromino(tetromino);
        switch (key) {
            case KEY_NONE: break;
            case KEY_LEFT: tetromino = MoveTetromino(tetromino, tetromino.x - 1, tetromino.y); break;
            case KEY_RIGHT: tetromino = MoveTetromino(tetromino, tetromino.x + 1, tetromino.y); break;
            case KEY_ROTATE: tetromino = RotateTetromino(tetromino); break;
            case KEY_HARD_DROP: tetromino = HardDrop(tetromino); break;
            case KEY_QUIT: break;
        }

        EraseTetromino(tetromino);
    }
}
