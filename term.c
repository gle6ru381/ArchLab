#include "term.h"
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>

int mt_clrcr()
{
    printf("\E[H\E[J");
    return 0;
}

int mt_gotoXY(int row, int column)
{
    int t_row, t_column;

    if (mt_getscreensize(&t_row, &t_column))
        return -1;
    if (t_row < row || t_column < column)
        return -1;

    printf("\E[%d;%dH", row, column);
    return 0;
}

int mt_getscreensize(int* rows, int* columns)
{
    struct winsize size;

    int retval = ioctl(1, TIOCGWINSZ, &size);
    if (retval != 0)
        return -1;
    *rows = size.ws_row;
    *columns = size.ws_col;

    return 0;
}

int mt_setfgcolor(enum Colors color)
{
    printf("\E[3%dm", color);
    return 0;
}

int mt_setbgcolor(enum Colors color)
{
    printf("\E[4%dm", color);
    return 0;
}
