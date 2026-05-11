// console.c
#include "console.h"
#include <stdio.h>

#ifdef _WIN32

#include <windows.h>
#include <conio.h>

void GotoXY(int x, int y) {
    COORD Pos;
    Pos.X = x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void CursorView(char show) {
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

void TextColor(Color color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#else

#include <unistd.h>

void GotoXY(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

void CursorView(char show) {
    if (show)
        printf("\033[?25h");
    else
        printf("\033[?25l");
    fflush(stdout);
}

void TextColor(Color color) {
    printf("\033[%dm", color);
    fflush(stdout);
}

#endif