#include "mainloop.h"
#include "mysimplecomputer.h"
#include "bigchars.h"
#include "readkey.h"
#include <stdio.h>

static struct Cell {
    uint8 posRow;
    uint8 posCol;
} currCell;

static inline void setDefaultColor()
{
    mt_setbgcolor(Color_White);
    mt_setfgcolor(Color_Black);
}

static inline void repaintCell()
{
    drawMemory();
    drawBigCell();
}

int loop_exec()
{
    currCell.posCol = 0;
    currCell.posRow = 0;
    setDefaultColor();

    fillContext();
    drawMemory();
    drawFlagsWin();
    drawKeys();
    drawAccumulator();
    drawOperationWin();
    drawInstructionCounter();
    drawBigCell();

    mt_gotoXY(25, 1);
    enum Keys key;
    while (1) {
        rk_readKey(&key);

        switch (key) {
        case KEY_right: {
            if (currCell.posCol < 9) {
                currCell.posCol++;
                repaintCell();
            }
            break;
        }
        case KEY_down: {
            if (currCell.posRow < 9) {
                currCell.posRow++;
                repaintCell();
            }
            break;
        }
        case KEY_left: {
            if (currCell.posCol > 0) {
                currCell.posCol--;
                repaintCell();
            }
            break;
        }
        case KEY_up: {
            if (currCell.posRow > 0) {
                currCell.posRow--;
                repaintCell();
            }
            break;
        }
        case KEY_s: {
            rk_mytermsave();
            break;
        }
        case KEY_l: {
            rk_mytermrestore();
            break;
        }
        }

        mt_gotoXY(25, 1);
    }
}

static inline void setIllegalStrMem(char str[6])
{
    str[0] = '-';
    str[1] = '0';
    str[2] = '0';
    str[3] = '0';
    str[4] = '0';
    str[5] = '\0';
}

static void printMemInd(int idx)
{
    int memVal, command, operand, retval;
    char printBuff[6];

    retval = sc_memoryGet(idx, &memVal);
    if (retval != 0) {
        setIllegalStrMem(printBuff);
        goto end_print;
    }
    retval = sc_commandDecode(memVal, &command, &operand);
    if (retval != 0) {
        setIllegalStrMem(printBuff);
        goto end_print;
    }

    printBuff[0] = '+';
    printBuff[1] = command / 10 + 0x30;
    printBuff[2] = command % 10 + 0x30;
    printBuff[3] = operand / 10 + 0x30;
    printBuff[4] = operand % 10 + 0x30;
    printBuff[5] = '\0';

    end_print:
    printf("%s", printBuff);
}

static void printFlagsReg()
{
    int regVal;
    char printBuff[5] = {' ', ' ', ' ', ' ', ' '};

    sc_regGet(Sc_Overflow, &regVal);
    if (regVal)
        printBuff[0] = 'O';
    sc_regGet(Sc_ZeroError, &regVal);
    if (regVal)
        printBuff[1] = 'Z';
    sc_regGet(Sc_SegFault, &regVal);
    if (regVal)
        printBuff[2] = 'S';
    sc_regGet(Sc_ClockIgnore, &regVal);
    if (regVal)
        printBuff[3] = 'C';
    sc_regGet(Sc_IllInstr, &regVal);
    if (regVal)
        printBuff[4] = 'I';

    for (int i = 0; i < 5; i++) {
        putchar(printBuff[i]);
        putchar(' ');
    }
}

void fillContext()
{
    int height, width;
    mt_getscreensize(&height, &width);

    mt_gotoXY(0, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            putchar(' ');
        putchar('\n');
    }
}

void drawMemory()
{
    bc_box(1, 1, 61, 12);
    mt_gotoXY(1, 26);
    printf(" Memory ");
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            mt_gotoXY(row + 2, (col * 6) + 2);
            printMemInd(row * 10 + col);
            putchar(' ');
        }
    }
    mt_gotoXY(currCell.posRow + 2, (currCell.posCol * 6) + 2);
    mt_setbgcolor(Color_Black);
    mt_setfgcolor(Color_White);
    printMemInd(currCell.posRow * 10 + currCell.posCol);
    setDefaultColor();
    putchar(' ');
}

void drawAccumulator()
{
    const int offsetCol = 63;

    bc_box(1, offsetCol, 20, 3);
    mt_gotoXY(1, 4 + offsetCol);
    printf(" accumulator ");
    mt_gotoXY(2, 8 + offsetCol);
    printf("+0000");
}

void drawInstructionCounter()
{
    const int offCol = 63;
    const int offRow = 4;

    bc_box(offRow, offCol, 20, 3);
    mt_gotoXY(offRow, 1 + offCol);
    printf(" intructionCounter ");
    mt_gotoXY(offRow + 1, 8 + offCol);
    printf("+0000");
}

void drawOperationWin()
{
    const int offCol = 63;
    const int offRow = 7;

    bc_box(offRow, offCol, 20, 3);
    mt_gotoXY(offRow, 5 + offCol);
    printf(" Operation ");
    mt_gotoXY(offRow + 1, 7 + offCol);
    printf("+00 : 00");
}

void drawFlagsWin()
{
    const int offCol = 63;
    const int offRow = 10;

    bc_box(offRow, offCol, 20, 3);
    mt_gotoXY(offRow, 7 + offCol);
    printf(" Flags ");
    mt_gotoXY(offRow + 1, 8 + offCol);
    printFlagsReg();
}

void drawKeys()
{
    const int offCol = 47;
    const int offRow = 13;

    bc_box(offRow, offCol, 36, 10);
    mt_gotoXY(offRow, offCol + 1);
    printf(" Keys: ");
    mt_gotoXY(offRow + 1, offCol + 1);
    printf("l - load");
    mt_gotoXY(offRow + 2, offCol + 1);
    printf("s - save");
    mt_gotoXY(offRow + 3, offCol + 1);
    printf("r - run");
    mt_gotoXY(offRow + 4, offCol + 1);
    printf("t - step");
    mt_gotoXY(offRow + 5, offCol + 1);
    printf("i - reset");
    mt_gotoXY(offRow + 6, offCol + 1);
    printf("F5 - accumulator");
    mt_gotoXY(offRow + 7, offCol + 1);
    printf("F6 - instructionCounter");
}

void getPlus(int val[2])
{
    val[0] = 0b11111111000110000001100000011000;
    val[1] = 0b00011000000110000001100011111111;
}

void getZero(int val[2])
{
    val[0] = 0b10000001100000011000000111111111;
    val[1] = 0b11111111100000011000000110000001;
}

void getOne(int val[2])
{
    val[0] = 0b01100000011000000110000001100000;
    val[1] = 0b01100000011000000110000001100000;
}

void getTwo(int val[2])
{
    val[0] = 0b11000000110000001100000011111111;
    val[1] = 0b11111111000000110000001111111111;
}

void getThree(int val[2])
{
    val[0] = 0b11111111110000001100000011111111;
    val[1] = 0b11111111110000001100000011111111;
}

void getFour(int val[2])
{
    val[0] = 0b11111111110000111100001111000011;
    val[1] = 0b11000000110000001100000011000000;
}

void getFive(int val[2])
{
    val[0] = 0b11111111000000110000001111111111;
    val[1] = 0b11111111110000001100000011000000;
}

void getSix(int val[2])
{
    val[0] = 0b11111111000000110000001111111111;
    val[1] = 0b11111111110000111100001111000011;
}

void getSeven(int val[2])
{
    val[0] = 0b00110000011000001100000011111111;
    val[1] = 0b00000011000001100000110000011000;
}

void getEight(int val[2])
{
    val[0] = 0b11111111110000111100001111111111;
    val[1] = 0b11111111110000111100001111111111;
}

void getNine(int val[2])
{
    val[0] = 0b11111111100000011000000111111111;
    val[1] = 0b00011000001100000110000011000000;
}

void getMinus(int val[2])
{
    val[0] = 0b11111111000000000000000000000000;
    val[1] = 0b00000000000000000000000011111111;
}

void choiseBigVal(int val, int retVal[2])
{
    switch (val) {
    case 0:
        getZero(retVal);
        return;
    case 1:
        getOne(retVal);
        return;
    case 2:
        getTwo(retVal);
        return;
    case 3:
        getThree(retVal);
        return;
    case 4:
        getFour(retVal);
        return;
    case 5:
        getFive(retVal);
        return;
    case 6:
        getSix(retVal);
        return;
    case 7:
        getSeven(retVal);
        return;
    case 8:
        getEight(retVal);
        return;
    case 9:
        getNine(retVal);
        return;
    }
}

#define printDef(val, row, col) bc_printbigchar((val), (row), (col), \
    Color_White, Color_Black)

void drawBigCell()
{
    const int offRow = 13;
    int memVal, command, operand;
    int offCol = 2;
    int val[2];

    bc_box(offRow, 1, 45, 10);

    sc_memoryGet(currCell.posRow * 10 + currCell.posCol, &memVal);
    if (sc_commandDecode(memVal, &command, &operand) != 0) {
        command = 0;
        operand = 0;
        getMinus(val);
        printDef(val, offRow + 1, offCol);
        offCol += 9;
        getZero(val);
        printDef(val, offRow + 1, offCol);
        offCol += 9;
        getZero(val);
        printDef(val, offRow + 1, offCol);
        offCol += 9;
        getZero(val);
        printDef(val, offRow + 1, offCol);
        offCol += 9;
        getZero(val);
        printDef(val, offRow + 1, offCol);
    } else {
    getPlus(val);
    printDef(val ,offRow + 1, offCol);
    offCol += 9;
    choiseBigVal(command / 10, val);
    printDef(val, offRow + 1, offCol);
    offCol += 9;
    choiseBigVal(command % 10, val);
    printDef(val, offRow + 1, offCol);
    offCol += 9;
    choiseBigVal(operand / 10, val);
    printDef(val, offRow + 1, offCol);
    offCol += 9;
    choiseBigVal(operand % 10, val);
    printDef(val, offRow + 1, offCol);
    }
}
