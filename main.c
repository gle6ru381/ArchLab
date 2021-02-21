#include "mysimplecomputer.h"
#include "bigchars.h"
#include <stdio.h>

#define printl(str, ...) printf(str"\n", __VA_ARGS__)

int main()
{
    sc_memoryInit();
    sc_regInit();
    int command;
    int retval = sc_commandEncode(10, 55, &command);
    printl("encode result: %d", retval);
    retval = sc_memorySet(0, command);
    printl("set memory result: %d", retval);
    retval = sc_commandEncode(11, 100, &command);
    printl("encode result: %d", retval);
    retval = sc_memorySet(1, command);
    printl("set memory result: %d", retval);
    printf("save result: %d\n", sc_memorySave("saveFile.bin"));
    sc_memoryInit();
    printf("load result: %d\n", sc_memoryLoad("saveFile.bin"));
    retval = sc_memoryGet(0, &command);
    printl("get result: %d", retval);
    int commandCode;
    int operand;
    retval = sc_commandDecode(command, &commandCode, &operand);
    printl("decode result: %d", retval);
    printf("code: %d, operand: %d\n", (uint16)commandCode, (uint16)operand);
    retval = sc_memoryGet(1, &command);
    printl("get result: %d", retval);
    sc_commandDecode(command, &commandCode, &operand);
    printf("code %d, operand: %d\n", commandCode, operand);

    mt_setbgcolor(Color_2);
    mt_setfgcolor(Color_6);
    int row, col;
    mt_getscreensize(&row, &col);
    printf("console size: %d, %d\n", row, col);
    mt_gotoXY(1, 5);
    printf("some text");
    mt_gotoXY(row, 0);
    mt_setbgcolor(Color_7);
    mt_setfgcolor(Color_8);

//    printf("\E(0");
//    printf("A=a\n\nF=f\n\nG=g\n\nI=i\n\nJ=j\n\nK=k\n\nL=l\n\n"
//           "M=m\n\nN=n\n\nO=o\n\nP=p\n\nQ=q\n\nR=r\n\nS=s\n\n"
//           "T=t\n\nU=u\n\nV=v\n\nW=w\n\nX=x\n\nY=y\n\nZ=z\n\n"
//           "FL={\n\nPS=|\n\nFR=}\n\nTILD=~\n\n");
    mt_clrcr();
    bc_box(5, 5, 9, 10);
    int sym[2];
    sym[0] = 0b10101010010101011010101001010101;
    sym[1] = 0b10101010010101011010101001010101;
    bc_setbigcharpos(sym, 0, 1, 1);
    bc_setbigcharpos(sym, 0, 3, 1);
    bc_printbigchar(sym, 6, 6, Color_8, Color_2);
    mt_gotoXY(0, 0);
    return 0;
}
