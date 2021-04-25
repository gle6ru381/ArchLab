#include "command.h"

union CommandForm {
    int16_t buff;
struct Command {
    uint8 operand : 7;
    uint8 operationCode : 7;
    uint8 commandB : 1;
} command;
};

static int validateCommand(int command)
{
    if (command < Sc_Priv_FirstCode || command > Sc_Priv_LastCode)
        return 0;
    switch(command) {
    case 12 ... 19:
    case 22 ... 29:
    case 34 ... 39:
    case 44 ... 50:
        return 0;
    default:
        return 1;
    }
}

static int validateOperand(int operand)
{
    uint8 mask = 1 << 7;
    if (operand >= mask || operand < 0)
        return 0;
    return 1;
}

#include <stdio.h>

int sc_commandEncode(int command, int operand, int* value)
{
    if (!validateCommand(command))
        return -1;
    if (!validateOperand(operand))
        return -2;

    union CommandForm form;
    form.buff = 0;
    form.command.commandB = 0;
    form.command.operationCode = command;
    form.command.operand = operand;

    *value = (int)form.buff;
    return 0;
}

int sc_commandDecode(int value, int* command, int* operand)
{
    union CommandForm form;
    form.buff = (int16_t)value;
    if (form.command.commandB) {
        return -1;
    }
    if (!validateCommand(form.command.operationCode)) {
        //sc_regSet(Sc_IllInstr, 1);
        return -2;
    }
    if (!validateOperand(form.command.operand))
        return -3;
    *command = form.command.operationCode;
    *operand = form.command.operand;
    return 0;
}

int sc_makeValue(int val)
{
    union CommandForm form;
    form.buff = (int16_t)val;
    if (form.buff < 0) {
        uint16_t tmp = form.buff;
        tmp <<= 1;
        tmp >>= 1;
        form.buff = tmp;
    }
    form.command.commandB = 1;
    return form.buff;
}

int16_t sc_remValue(int val)
{
    union CommandForm form;
    form.buff = (int16_t)val;
    form.command.commandB = 0;
    int16_t tmp = form.buff << 1;
    if ((tmp) < 0) {
        form.buff = tmp >> 1;
    }
    return form.buff;
}
