#ifndef GAME_H
#define GAME_H

#define BOARD_ROWS 20
#define BOARD_COLS 10

// 보드 원점 (터미널 화면에서 보드가 시작되는 위치)
#define BOARD_ORIGIN_X 2
#define BOARD_ORIGIN_Y 1

extern int board[BOARD_ROWS][BOARD_COLS];

void RunGame(void);

#endif