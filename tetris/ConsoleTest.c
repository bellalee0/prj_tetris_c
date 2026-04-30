#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ARROW 224
#define SPACEBAR 32
#define ENTER 13

// 원하는 위치로 이동
void GotoXY(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 커서 숨기기
void CursorView(char show)
{
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

int x = 30;
int y = 10;

int main(void) {

	CursorView(0);
	GotoXY(x, y);

	while (1) {
		if (_kbhit()) {

			int nkey = _getch();

			if (nkey == ENTER) {
				GotoXY(x, y);
				printf("★");
				//system("cls");
			}

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
		}

	}

	return 0;
}