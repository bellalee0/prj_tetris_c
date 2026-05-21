#include "game.h"
#include "console.h"
#include "block.h"

#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
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

int board[BOARD_ROWS][BOARD_COLS];  // 0 = 빈칸, 1 이상 = 고정된 블록

// GetKey: 입력 키 인식
int GetKey() {

#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
    return -1;

#else
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
#endif
}


// DrawBorder: 게임 진행 영역 테두리 출력
void DrawBorder() {
    
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


// DrawBoard: 현재 보드 상태 출력
void DrawBoard() {
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
                GotoXY(activeBlock.x+j, activeBlock.y+i);
                TextColor(activeBlock.tetromino.color);
                printf("■");
            }
        }
    }
}

void EraseTetromino(ActiveBlock activeBlock) {

    GotoXY(activeBlock.x, activeBlock.y);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (activeBlock.tetromino.shape[i][j] == 2) {
                GotoXY(activeBlock.x + j, activeBlock.y + i);
                printf("  ");
            }
        }
    }
}

void RunGame(void) {

    CursorView(0);
    DrawBorder();

    ActiveBlock example = {
        .tetromino = BLOCK_O,
        .x = 10,
        .y = 1
    };

    DrawTetromino(example);
    Sleep(5000);
    EraseTetromino(example);
}


