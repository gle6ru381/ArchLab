#include "mainloop.h"
#include "mysimplecomputer.h"
#include "bigchars.h"
#include "readkey.h"
#include "process.h"
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

static struct Cell {
    uint8 posRow;
    uint8 posCol;
} currCell;

#define TimerInit(timer) \
    timer.it_value.tv_usec = 800000; \
    timer.it_value.tv_sec = 0; \
    timer.it_interval.tv_sec = 0; \
    timer.it_interval.tv_usec = 800000;

static int bbToS(int b1, int b2)
{
    return (b1 / 0x10) * (0x1000) + (b1 % 0x10) * (0x100) +
            (b2 / 0x10) * 0x10 + b2 % 0x10;
}

//void sTobb(uint16 s, int* b1, int* b2)
//{
//    *b1 = s >> 8;
//    *b2 = s % 0x100;
//}

static inline void repaintCell()
{
    drawMemory();
    drawBigCell();
    drawOperationWin();
}

static void timerTimeoutHandler(int a)
{
    (void)a;
    int clockFlag;
    sc_regGet(Sc_ClockIgnore, &clockFlag);
    if (clockFlag)
        return;
    CU();
    drawInstructionCounter(0);
    drawAccumulator();
    drawFlagsWin();
    repaintCell();
}

static void userSignalHandler(int a)
{
    (void)a;
    sc_regInit();
    sc_memoryInit();

    fillContext();
    drawMemory();
    drawFlagsWin();
    drawKeys();
    drawAccumulator();
    drawOperationWin();
    drawInstructionCounter(1);
    drawBigCell();
}

static inline void inputAccumulator();
static inline void inputCounter();

static inline void setDefaultColor()
{
    mt_setbgcolor(Color_White);
    mt_setfgcolor(Color_Black);
}

int loop_exec()
{
    currCell.posCol = 0;
    currCell.posRow = 0;
    setDefaultColor();

    rk_mytermregime(1, 0, 0, 0, 0);

    sc_regSet(Sc_IllInstr, 0);

    fillContext();
    drawMemory();
    drawFlagsWin();
    drawKeys();
    drawAccumulator();
    drawOperationWin();
    drawInstructionCounter(1);
    drawBigCell();

    //signal(SIGALRM, &timerTimeoutHandler);
    struct sigaction act;
    act.sa_handler = &timerTimeoutHandler;
    act.sa_flags = SA_RESTART;
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, NULL);

    act.sa_handler = &userSignalHandler;
    act.sa_flags = SA_RESTART;
    sigemptyset(&act.sa_mask);

    sigaction(SIGUSR1, &act, NULL);

    mt_gotoXY(25, 1);
    enum Keys key;
    while (1) {
        rk_readKey(&key);
//        int regVal;
//        sc_regGet(Sc_ClockIgnore, &regVal);
//        if (regVal)
//            continue;
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
        case KEY_r: {
            sc_regSet(Sc_ClockIgnore, 0);
            sc_regSet(Sc_IllInstr, 0);
            sc_regSet(Sc_Overflow, 0);
            sc_regSet(Sc_SegFault, 0);
            sc_regSet(Sc_ZeroError, 0);
            struct itimerval timer;
            TimerInit(timer);
            setitimer(ITIMER_REAL, &timer, NULL);
            break;
        }
        case KEY_t: {
            timerTimeoutHandler(0);
            break;
        }
        case KEY_s: {
            mt_gotoXY(25, 1);
            rk_mytermsave();
            mt_gotoXY(25, 1);
            printf("                                     \n"
                   "                                     ");
            getchar();
            break;
        }
        case KEY_l: {
            mt_gotoXY(25, 1);
            rk_mytermrestore();
            mt_gotoXY(25, 1);
            printf("                                       \n"
                   "                                       ");
            getchar();
            repaintCell();
            break;
        }
        case KEY_i: {
            raise(SIGUSR1);
            break;
        }
        case KEY_ENTER: {
            inputMemory(currCell.posRow * 10 + currCell.posCol);
            repaintCell();
            break;
        }
        case KEY_F5: {
			mt_gotoXY(25, 1);
			inputAccumulator();
			getchar();
			drawAccumulator();
			break;

        }
        case KEY_F6: {
			mt_gotoXY(25, 1);
			inputCounter();
			getchar();
            drawInstructionCounter(0);
			break;
		}
        case KEY_Illegal:
            break;
        }
        drawInstructionCounter(0);
        drawAccumulator();
        drawFlagsWin();
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

static void getMemBuff(char buff[6], int const command, int const operand)
{
    sprintf(buff, "+%02X%02X", command, operand);
//	buff[0] = '+';
//    buff[1] = command / 10 + 0x30;
//    buff[2] = command % 10 + 0x30;
//    buff[3] = operand / 10 + 0x30;
//    buff[4] = operand % 10 + 0x30;
//    buff[5] = '\0';
}

static void getMemSepBuff(char buff[9], int const command, int const operand)
{
    sprintf(buff, "+%02X : %02X", command, operand);
//    buff[0] = '+';
//    buff[1] = command / 16 + 0x30;
//    buff[2] = command % 16 + 0x30;
//    buff[3] = ' ';
//    buff[4] = ':';
//    buff[5] = ' ';
//    buff[6] = operand / 16 + 0x30;
//    buff[7] = operand % 16 + 0x30;
//    buff[8] = '\0';
}

static void getOperandBuff(char buff[6], int const operand)
{
    uint16_t val = (uint16_t)sc_remValue(operand);
    sprintf(buff, "-%04X", val);
//    buff[0] = '-';
//    buff[1] = '0';
//    buff[2] = '0';
//    buff[3] = operand / 16 + 0x30;
//    buff[4] = operand % 16 + 0x30;
//    buff[5] = '\0';
}

static void getOperandSepBuff(char buff[9], int operand)
{
    operand = sc_remValue(operand);
    sprintf(buff, "-%02X : %02X", operand >> 8, operand % 0x100);
//    buff[0] = '-';
//    buff[1] = '0';
//    buff[2] = '0';
//    buff[3] = ' ';
//    buff[4] = ':';
//    buff[5] = ' ';
//    buff[6] = operand / 16 + 0x30;
//    buff[7] = operand % 16 + 0x30;
//    buff[8] = '\0';
}

static void getMemFromBuff(char buff[5], int* command, int* operand)
{
    sscanf(buff, "%02X%02X", command, operand);
//    *command = (buff[0] - 0x30) * 16;
//	*command += (buff[1] - 0x30);
//	*operand = (buff[2] - 0x30) * 10;
//	*operand += (buff[3] - 0x30);
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
        getOperandBuff(printBuff, memVal);
        //setIllegalStrMem(printBuff);
    } else {
        getMemBuff(printBuff, command, operand);
    }
    //printBuff[0] = '+';
    //printBuff[1] = command / 10 + 0x30;
    //printBuff[2] = command % 10 + 0x30;
    //printBuff[3] = operand / 10 + 0x30;
    //printBuff[4] = operand % 10 + 0x30;
    //printBuff[5] = '\0';
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

void fillRow(int row)
{
    mt_gotoXY(row, 0);
    for (int i = 0; i < 100; i++) {
        putchar(' ');
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
    uint16 val = sc_accumGet();
    int comm, operand;
    char buff[6];
    
    int retVal = sc_commandDecode(val, &comm, &operand);

    bc_box(1, offsetCol, 20, 3);
    mt_gotoXY(1, 4 + offsetCol);
    printf(" accumulator ");
    mt_gotoXY(2, 8 + offsetCol);
    
    if (retVal == 0)
        getMemBuff(buff, comm, operand);
    else
        getOperandBuff(buff, val);
    
    printf("%s", buff);
}

void drawInstructionCounter(int fullPaint)
{
    const int offCol = 63;
    const int offRow = 4;
    uint16 counterVal = sc_counterGet();
    int val;
    sc_memoryGet(counterVal, &val);
    int comm, operand;
    char buff[6];
    
    int retVal = sc_commandDecode(val, &comm, &operand);
    if (retVal != 0) {
        getOperandBuff(buff, val);
    } else {
        getMemBuff(buff, comm, operand);
    }

    if (fullPaint) {
        bc_box(offRow, offCol, 20, 3);
        mt_gotoXY(offRow, 1 + offCol);
        printf(" intructionCounter ");
    }
    mt_gotoXY(offRow + 1, 8 + offCol);
    
    //getMemBuff(buff, comm, operand);
    
    printf("%s", buff);
}

void drawOperationWin()
{
    const int offCol = 63;
    const int offRow = 7;

    bc_box(offRow, offCol, 20, 3);
    mt_gotoXY(offRow, 5 + offCol);
    printf(" Operation ");
    mt_gotoXY(offRow + 1, 7 + offCol);
    int memInd;
    sc_memoryGet(currCell.posRow * 10 + currCell.posCol, &memInd);

    int comm, operand;
    char buff[9];

    int retVal = sc_commandDecode(memInd, &comm, &operand);
    if (retVal != 0) {
        getOperandSepBuff(buff, memInd);
    } else {
        getMemSepBuff(buff, comm, operand);
    }

    printf("%s", buff);
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

void getAChar(int val[2])
{
    val[0] = 2118269952;
    val[1] = 4342338;
}

void getBChar(int val[2])
{
    val[0] = 1044528640;
    val[1] = 4080194;
}

void getCChar(int val[2])
{
    val[0] = 37895168;
    val[1] = 3949058;
}

void getDChar(int val[2])
{
    val[0] = 1111637504;
    val[1] = 4080194;
}

void getEChar(int val[2])
{
    val[0] = 2114092544;
    val[1] = 8258050;
}

void getFChar(int val[2])
{
    val[0] = 33717760;
    val[1] = 131646;
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
    case 10:
        getAChar(retVal);
        return;
    case 11:
        getBChar(retVal);
        return;
    case 12:
        getCChar(retVal);
        return;
    case 13:
        getDChar(retVal);
        return;
    case 14:
        getEChar(retVal);
        return;
    case 15:
        getFChar(retVal);
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
    choiseBigVal(command / 16, val);
    printDef(val, offRow + 1, offCol);
    offCol += 9;
    choiseBigVal(command % 16, val);
    printDef(val, offRow + 1, offCol);
    offCol += 9;
    choiseBigVal(operand / 16, val);
    printDef(val, offRow + 1, offCol);
    offCol += 9;
    choiseBigVal(operand % 16, val);
    printDef(val, offRow + 1, offCol);
    }
}

static inline void inputAccumulator()
{
    printf("Введите значение:\n");
    rk_mytermregime(0, 0, 0, 1, 0);
    int command, operand, result;
    scanf("%02X%02X", &command, &operand);
    
    int retval = sc_commandEncode(command, operand, &result);
    if (retval != 0) {
        result = bbToS(command, operand);
    }
//		printf("Неверное значение");
//		goto end_func;
//	}
    retval = sc_accumSet((uint16)result);
    if (retval != 0) {
        printf("Неверное значение");
    }
	
	rk_mytermregime(1, 0, 0, 0, 0);
}

static inline void inputCounter()
{
	printf("Введите значение:\n");
    rk_mytermregime(0, 0, 0, 1, 0);
    int addr;
    scanf("%d", &addr);

    
    if (addr >= SC_MEMORY_SIZE) {
		printf("Неверное значение");
		goto end_func;
	}
    sc_counterSet((uint16)addr);
	
	end_func:
	rk_mytermregime(1, 0, 0, 0, 0);
}
