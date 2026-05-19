#include "board.h"
#include <stdio.h>
#include "console.h"

#define BOARD_ROWS 20
#define BOARD_COLS 10

int board[BOARD_ROWS][BOARD_COLS];

void DrawBoarder(void) {

	for (int i = 0; i < BOARD_ROWS; i++) {

		for (int j = 0; j < BOARD_COLS; j++) {
			
			if (i == 0 || i == BOARD_ROWS - 1) {
				GotoXY(j * 2, i);
				printf("-");
			}
			else {
				if (j == 0 || j == BOARD_COLS - 1) {
					GotoXY(j * 2, i);
					printf("|");
				}
			}
		}
	}
	
}