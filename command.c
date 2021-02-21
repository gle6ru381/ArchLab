#include "command.h"

union CommandForm {
    uint16 buff;
struct Command {
    uint8 commandB : 1;
    uint8 operationCode : 7;
    uint8 operand : 7;
} command;
};

static int validateCommand(int command)
{
    if (command == 0)
        return 1;
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

    printf("form: %d, %d\n", form.command.operationCode, form.command.operand);

    *value = (int)form.buff;
    return 0;
}

int sc_commandDecode(int value, int* command, int* operand)
{
    union CommandForm form;
    form.buff = (uint16)value;
    if (form.command.commandB)
        return -1;
    if (!validateCommand(form.command.operationCode)) {
        sc_regSet(Sc_IllInstr, 1);
        return -2;
    }
    if (!validateOperand(form.command.operand))
        return -3;
    *command = form.command.operationCode;
    *operand = form.command.operand;
    return 0;
}
