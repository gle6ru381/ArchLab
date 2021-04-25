#ifndef COMMAND_H
#define COMMAND_H

#include "memory.h"

typedef enum {
    Sc_Priv_FirstCode = 10,
    Sc_Priv_LastCode = 76
} Sc_CommandCodes;

//!Кодирует команду с указанным номеров
//! и операндом и помещает результат в value
int sc_commandEncode(int command, int operand, int* value);

//!декодирует значение как команду Simple Computer.
int sc_commandDecode(int value, int* command, int* operand);

int sc_makeValue(int val);
int16_t sc_remValue(int val);

#endif
