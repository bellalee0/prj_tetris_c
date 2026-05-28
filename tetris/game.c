#include "game.h"
#include "console.h"
#include "block.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// DrawTetromino: 블록 출력
void DrawTetromino(ActiveBlock activeBlock) {

    GotoXY(activeBlock.x, activeBlock.y);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (activeBlock.tetromino.shape[i][j] == 2) {
                GotoXY(activeBlock.x + j * 2, activeBlock.y+i);
                TextColor(activeBlock.tetromino.color);
                printf("■");
            }
        }
    }
}

// EraseTetromino: 블록 지우기
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
}

// SpawnTetromino: 7종 중 랜덤 블록 생성
ActiveBlock SpawnTetromino() {

    srand(time(NULL));

    int block_type = rand() % 7;

    Tetromino tetromino;

    switch (block_type) {
        case 0:
            tetromino = BLOCK_I;
            break;
        case 1:
            tetromino = BLOCK_O;
            break;
        case 2:
            tetromino = BLOCK_T;
            break;
        case 3:
            tetromino = BLOCK_J;
            break;
        case 4:
            tetromino = BLOCK_L;
            break;
        case 5:
            tetromino = BLOCK_S;
            break;
        case 6:
            tetromino = BLOCK_Z;
            break;
    }

    ActiveBlock activeBlock = {
        .tetromino = tetromino,
        .x = BOARD_ORIGIN_X + BOARD_COLS / 2,
        .y = BOARD_ORIGIN_Y
    };

    return activeBlock;
}

void RunGame(void) {

    CursorView(0);
    DrawBorder();

    for (int i = 0; i < 10; i++) {
        ActiveBlock tetromino = SpawnTetromino();

        DrawTetromino(tetromino);
        
        #ifdef _WIN32
        Sleep(5000);
        #else
        sleep(4);
        #endif

        EraseTetromino(tetromino);
    }
}


