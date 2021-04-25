#include "memory.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#define MAX_VALUE (1 << 15) - 1
static uint16 memory[SC_MEMORY_SIZE];
static Sc_FlagsReg flagReg;
static int16_t accumReg = 0;
static uint16 counterReg = 0;

int sc_memoryInit()
{
  return memset(memory, 0, SC_MEMORY_SIZE), 0;
}

int sc_memorySet(int address, int value)
{
    if (address > SC_MEMORY_SIZE) {
        flagReg |= Sc_Overflow;
        return -1;
    }
    if (value > MAX_VALUE && ((uint16)value >> 15) == 0) {
        flagReg |= Sc_Overflow;
        return -2;
    }
    memory[address] = (uint16)value;
    return 0;
}

int sc_memoryGet(int addr, int* val)
{
    if (addr > SC_MEMORY_SIZE) {
        flagReg |= Sc_Overflow;
        return -1;
    }
    if (!val)
        return -3;

    *val = memory[addr];
    return 0;
}

int sc_memorySave(char* file)
{
    int fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1)
        return -1;
    int retVal = write(fd, memory, sizeof(memory));
    if (retVal == -1) {
        perror("");
        return -2;
    }
    else if (retVal != sizeof (memory))
        return -3;
    else
        return 0;
}

int sc_memoryLoad(char* file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
        return -1;
    int retVal = read(fd, memory, sizeof(memory));
    if (retVal == -1) {
        perror("");
        return -2;
    }
    if (retVal != sizeof (memory))
        return -3;
    return 0;
}

int sc_regInit()
{
    flagReg = 0;
    accumReg = 0;
    counterReg = 0;
    return 0;
}

int sc_regSet(int reg, int val)
{
    if (reg > SC_Priv_Last || reg < 1)
        return -1;
    if (val > 1 || val < 0)
        return -2;

    if (val) {
        flagReg |= reg;
    } else
        flagReg &= ~reg;
    return 0;
}

int sc_regGet(int reg, int* val)
{
    if (reg > SC_Priv_Last || reg < 1)
        return -1;
    if (!val)
        return -2;

    *val = flagReg & reg ? 1 : 0;
    return 0;
}

int sc_accumSet(int16_t val)
{
//    if (val > MAX_VALUE) {
//        return -1;
//    }
    accumReg = val;
    return 0;
}

int16_t sc_accumGet()
{
    return accumReg;
}

int sc_counterSet(uint16 val)
{
    if (val > MAX_VALUE)
        return -1;
    counterReg = val;
    return 0;
}

uint16 sc_counterGet()
{
  return counterReg;
}

#include <stdio.h>

void sc_printFlagReg()
{
  printf("%X\n", flagReg);
}
