#include "render.h"
#include "game.h"
#include "console.h"

#include <stdio.h>

void DrawBorder(void) {

    TextColor(WHITE);

    GotoXY(BOARD_ORIGIN_X, BOARD_ORIGIN_Y);
    printf("┌");
    for (int i = 0; i < BOARD_COLS; i++) printf("──");
    printf("┐");

    for (int row = 0; row < BOARD_ROWS; row++) {
        GotoXY(BOARD_ORIGIN_X, BOARD_ORIGIN_Y + 1 + row);
        printf("│");
        GotoXY(BOARD_ORIGIN_X + 1 + BOARD_COLS * 2, BOARD_ORIGIN_Y + 1 + row);
        printf("│");
    }

    GotoXY(BOARD_ORIGIN_X, BOARD_ORIGIN_Y + 1 + BOARD_ROWS);
    printf("└");
    for (int i = 0; i < BOARD_COLS; i++) printf("──");
    printf("┘");
}

void DrawBoard(void) {
    for (int row = 0; row < BOARD_ROWS; row++) {
        GotoXY(BOARD_ORIGIN_X + 1, BOARD_ORIGIN_Y + 1 + row);
        for (int col = 0; col < BOARD_COLS; col++) {
            if (board[row][col] == 2)
                printf("■");
            else
                printf("  ");
        }
    }
}

void DrawTetromino(ActiveBlock activeBlock) {

    GotoXY(activeBlock.x, activeBlock.y);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (activeBlock.tetromino.shape[i][j] == 2) {
                GotoXY(activeBlock.x + j * 2, activeBlock.y + i);
                TextColor(activeBlock.tetromino.color);
                printf("■");
            }
        }
    }
    fflush(stdout);
}

void EraseTetromino(ActiveBlock activeBlock) {

    GotoXY(activeBlock.x, activeBlock.y);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (activeBlock.tetromino.shape[i][j] == 2) {
                GotoXY(activeBlock.x + j * 2, activeBlock.y + i);
                printf("  ");
            }
        }
    }
    fflush(stdout);
}
