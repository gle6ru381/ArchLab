#include "mysimplecomputer.h"
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
    return 0;
}
