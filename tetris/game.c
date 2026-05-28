#include "game.h"
#include "console.h"
#include "render.h"
#include "logic.h"

#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int board[BOARD_ROWS][BOARD_COLS];

void RunGame(void) {

    srand(time(NULL));
    CursorView(0);
    DrawBorder();

    for (int i = 0; i < 10; i++) {
        ActiveBlock tetromino = SpawnTetromino();

        DrawTetromino(tetromino);

        #ifdef _WIN32
        Sleep(5000);
        #else
        sleep(5);
        #endif

        EraseTetromino(tetromino);
    }
}
