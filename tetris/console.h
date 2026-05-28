#ifndef CONSOLE_H
#define CONSOLE_H

typedef enum {
	BLACK,
	DARKBLUE,
	DARKGREEN,
	DARKSKYBLUE,
	DARKRED,
	DARKPURPLE,
	DARKYELLOW,
	GRAY,
	DARKGRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	PURPLE,
	YELLOW,
	WHITE
} Color;

extern int ANSI_COLOR[16];

void GotoXY(int x, int y);
void CursorView(char show);
void TextColor(Color color);

#endif