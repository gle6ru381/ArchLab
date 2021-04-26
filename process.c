#include "process.h"
#include "mysimplecomputer.h"
#include "bigchars.h"
#include "readkey.h"
#include <stdio.h>
#include <signal.h>

static void outMemory(int addr)
{
    mt_gotoXY(25, 1);
    printf("Значение:\n");
    int val;
    sc_memoryGet(addr, &val);
    val &= ~(1 << 15);
    printf("%04X", val);
}

static void loadAccum(int addr)
{
    int val;
    sc_memoryGet(addr, &val);
    sc_accumSet(val);
}

static void storeAccum(int addr)
{
    int val = sc_accumGet();
    sc_memorySet(addr, val);
}


static int addFunc(int operand)
{
    int val;
    sc_memoryGet(operand, &val);
    val = sc_remValue(val);
    int accum = sc_remValue(sc_accumGet());
    int total = accum + val;
    return sc_accumSet(sc_makeValue(total));
}

static int subFunc(int operand)
{
    int val;
    sc_memoryGet(operand, &val);
    val = sc_remValue(val);
    int accum = sc_remValue(sc_accumGet());
    int total = (accum - val);
    return sc_accumSet(sc_makeValue(total));
}

static int divideFunc(int operand)
{
    int val;
    sc_memoryGet(operand, &val);
    val = sc_remValue(val);
    if (val == 0)
        return -1;
    int accum = sc_remValue(sc_accumGet());
    int total = accum / val;
    return sc_accumSet(sc_makeValue(total));
}

static int mulFunc(int operand)
{
    int val;
    sc_memoryGet(operand, &val);
    val = sc_remValue(val);
    int accum = sc_remValue(sc_accumGet());
    int total = accum * val;
    return sc_accumSet(sc_makeValue(total));
}

static int ALU(int command, int operand)
{
    switch (command) {
    case 30:
        return addFunc(operand);
        break;
    case 31:
        return subFunc(operand);
        break;
    case 32:
        return divideFunc(operand);
        break;
    case 33:
        return mulFunc(operand);
        break;
    }
    return -1;
}

static int readFunc(int operand)
{
    if (operand >= SC_MEMORY_SIZE)
        return -1;

    inputMemory(operand);
    return 0;
}

static int writeFunc(int operand)
{
    if (operand >= SC_MEMORY_SIZE)
        return -1;

    outMemory(operand);
    return 0;
}

static int loadFunc(int operand)
{
    if (operand >= SC_MEMORY_SIZE)
        return -1;
    loadAccum(operand);
    return 0;
}

static int storeFunc(int operand)
{
    if (operand >= SC_MEMORY_SIZE)
        return -1;
    storeAccum(operand);
    return 0;
}

static int jumpFunc(int operand)
{
    if (operand >= SC_MEMORY_SIZE)
        return -1;
    sc_counterSet(operand);
    return 0;
}

static int jnegFunc(int operand)
{
    short val = sc_remValue(sc_accumGet());
    if (val < 0)
        sc_counterSet(operand);
    return 0;
}

static int jzFunc(int operand)
{
    short val = sc_remValue(sc_accumGet());
    if ((val) == 0)
        sc_counterSet(operand);
    return 0;
}

static int haltFunc(int a)
{
    (void)a;
    raise(SIGUSR1);
    return 0;
}

static int movrFunc(int operand)
{
    int accumVal = sc_accumGet();
    int memVal;
    sc_memoryGet(accumVal, &memVal);
    sc_memorySet(operand, memVal);
    return 0;
}

int CU()
{
    int memVal, comm, oper;
    uint16 counterAddr = sc_counterGet();
    sc_counterSet((counterAddr + 1) % SC_MEMORY_SIZE);

    sc_memoryGet(counterAddr, &memVal);
    int retVal = sc_commandDecode(memVal, &comm, &oper);
    if (retVal == -1) {
        sc_regSet(Sc_IllInstr, 1);
        sc_regSet(Sc_ClockIgnore, 1);
        return -1;
    }
    if (comm >= 30 && comm <= 33)
        ALU(comm, oper);
    else {
        switch (comm) {
        case 10:
            return readFunc(oper);
        case 11:
            return writeFunc(oper);
        case 20:
            return loadFunc(oper);
        case 21:
            return storeFunc(oper);
        case 40:
            return jumpFunc(oper);
        case 41:
            return jnegFunc(oper);
        case 42:
            return jzFunc(oper);
        case 43:
            return haltFunc(oper);
        case 72:
            return movrFunc(oper);
        default:
            return -1;
        }
    }
    return -1;
}

static int bbToS(int b1, int b2)
{
    return (b1 / 0x10) * (0x1000) + (b1 % 0x10) * (0x100) +
            (b2 / 0x10) * 0x10 + b2 % 0x10;
}

void inputMemory(int addr)
{
    sc_regSet(Sc_ClockIgnore, 1);
    mt_gotoXY(25, 1);
    rk_mytermregime(0, 0, 0, 1, 0);
    printf("Введите признак команды:\n");
    int commandB;
    scanf("%d", &commandB);
    int command, operand, result;
    printf("Введите значение:\n");
    scanf("%02X%02X", &command, &operand);
    int retval;
    if (commandB != 0) {
        result = bbToS(command, operand);
        result = sc_makeValue(result);
        retval = sc_memorySet((addr), result);
        if (retval != 0) {
            printf("Неверное значение\n");
        }
    } else {
        retval = sc_commandEncode(command, operand, &result);
        if (retval != 0) {
            printf("Неверное значение\n");
        } else {
            retval = sc_memorySet((addr), result);
            if (retval != 0) {
                printf("Неверное значение\n");
            }
        }
    }
    mt_gotoXY(25, 1);
    printf("                                        ");
    mt_gotoXY(26,1);
    printf("                                        ");
    mt_gotoXY(27, 1);
    printf("                                        ");
    mt_gotoXY(28, 1);
    printf("                                        ");

    rk_mytermregime(1, 0, 0, 0, 0);
    getchar();
    sc_regSet(Sc_ClockIgnore, 0);
}
