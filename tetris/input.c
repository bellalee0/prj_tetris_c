#include "input.h"

#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#endif

int GetKey(void) {

#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
    return -1;

#else
    struct timeval tv = { 0, 0 };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(1, &fds, NULL, NULL, &tv) <= 0) return -1;
    return getchar();
#endif
}
