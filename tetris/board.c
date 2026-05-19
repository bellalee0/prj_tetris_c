#include "board.h"
#include <stdio.h>
#include "console.h"

#define BOARD_ROWS 20
#define BOARD_COLS 10

// 보드 원점 (터미널 화면에서 보드가 시작되는 위치)
#define BOARD_ORIGIN_X 2
#define BOARD_ORIGIN_Y 1

int board[BOARD_ROWS][BOARD_COLS];  // 0 = 빈칸, 1 이상 = 고정된 블록

// DrawBorder: 게임 시작 시 1회 호출
void DrawBorder() {

    // 윗벽
    GotoXY(BOARD_ORIGIN_X, BOARD_ORIGIN_Y);
    printf("┌");
    for (int i = 0; i < BOARD_COLS; i++) printf("──");
    printf("┐");

    // 양 옆벽 (각 행마다)
    for (int row = 0; row < BOARD_ROWS; row++) {
        GotoXY(BOARD_ORIGIN_X, BOARD_ORIGIN_Y + 1 + row);
        printf("│");
        GotoXY(BOARD_ORIGIN_X + 1 + BOARD_COLS * 2, BOARD_ORIGIN_Y + 1 + row);
        printf("│");
    }

    // 아랫벽
    GotoXY(BOARD_ORIGIN_X, BOARD_ORIGIN_Y + 1 + BOARD_ROWS);
    printf("└");
    for (int i = 0; i < BOARD_COLS; i++) printf("──");
    printf("┘");
}

// DrawBoard: 게임 루프에서 매 프레임 호출
void DrawBoard() {
    for (int row = 0; row < BOARD_ROWS; row++) {
        GotoXY(BOARD_ORIGIN_X + 1, BOARD_ORIGIN_Y + 1 + row);
        for (int col = 0; col < BOARD_COLS; col++) {
            if (board[row][col] == 2)
                printf("■");  // 고정된 블록
            else
                printf("  ");  // 빈칸
        }
    }
}