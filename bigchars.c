#include "bigchars.h"
#include <stdio.h>
#include <unistd.h>

int bc_printA(char* str)
{
    printf("\E(0");
    printf("%s", str);
    printf("\E(B");
    return 0;
}

int bc_box(int row, int col, int width, int height)
{
    {
    int rows, cols;
    if (mt_getscreensize(&rows, &cols) != 0)
        return -1;
    if (rows < (row + width) || cols < (col + height))
        return -1;
    }
   mt_gotoXY(row, col);
   printf("\E(0");
   putchar('l');
   for (int i = 1; i < width; i++) {
       putchar('q');
   }
   putchar('k');
   mt_gotoXY(row + 1, col);
   for (int i = 1; i < (height - 1); i++) {
       putchar('x');
       mt_gotoXY(row + i, col + width);
       putchar('x');
       mt_gotoXY(row + i + 1, col);
   }
   putchar('m');
   for (int i = 1; i < width; i++)
       putchar('q');
   putchar('j');

   printf("\E(B");
   return 0;
}

int bc_printbigchar(int big[2], int row, int col,
    enum Colors colorbg, enum Colors colorfg)
{
    {
        int rows, cols;
        if (mt_getscreensize(&rows, &cols) != 0)
            return -1;
        if (rows < (row + 8) || cols < (col + 8))
            return -1;
    }
    mt_setbgcolor(colorbg);
    mt_setfgcolor(colorfg);

    printf("\E(0");
    for (int i = 0; i < 4; i++) {
        mt_gotoXY(row + i, col);
        for (int j = 0; j < 8; j++) {
            if (big[0] & BIT((8 * i) + j))
                putchar('a');
            else
                putchar(' ');
        }
    }
    for (int i = 0; i < 4; i++) {
        mt_gotoXY(row + i + 4, col);
        for (int j = 0; j < 8; j++) {
            if ( big[1] & BIT((8 * i) + j))
                putchar('a');
            else
                putchar(' ');
        }
    }
    printf("\E(B");
    return 0;
}

int bc_setbigcharpos(int* big, int row, int col, int val)
{
    if (row > 7 || col > 7)
        return -1;
    if (row < 4) {
        if (val) {
            big[0] |= BIT((row * 8) + col);
        } else {
            big[0] &= ~BIT((row * 8) + col);
        }
    } else {
        if (val) {
            big[1] |= BIT(((row - 4) * 8) + col);

        } else {
            big[1] &= ~BIT(((row - 4) * 8) + col);
        }
    }
    return 0;
}

int bc_getbigcharpos(int* big, int row, int col, int* val)
{
    if (row > 7 || col > 7)
        return -1;
    if (row < 4) {
        if (big[0] & BIT((row * 8) + col))
            *val = 1;
        else
            *val = 0;
    } else {
        if (big[1] * BIT(((row - 4) * 8) + col))
            *val = 1;
        else *val = 0;
    }
    return 0;
}

int bc_bigcharwrite(int fd, int* big, int count)
{
    if (fd == -1)
        return -1;
    for (int i = 0; i < count; i++) {
        int retval = write(fd, big + (i * 2), sizeof (int) * 2);
        if (retval != sizeof (int) * 2)
            return -1;
    }
    return 0;
}

int bc_bigcharread(int fd, int* big, int need_count, int* count)
{
    if (fd == -1)
        return -1;
    for (int i = 0; i < need_count; i++) {
        int retval = read(fd, big + (i * 2), sizeof (int) * 2);
        if (retval == 0) {
            *count = i;
            return 0;
        }
        if (retval != sizeof (int) * 2) {
            *count = 0;
            return -1;
        }
    }
    return 0;
}
