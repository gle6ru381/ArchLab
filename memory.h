#ifndef MEMORY_H
#define MEMORY_H

#include <string.h>
#include <inttypes.h>

#define SC_MEMORY_SIZE 100

typedef uint16_t uint16;
typedef uint8_t uint8;

typedef enum {
    //!Переполнение при выполнении
  Sc_Overflow = 1,
    //!Ошибка деления на 0
  Sc_ZeroError = Sc_Overflow << 1,
    //!Ошибка выхода за границы памяти
  Sc_SegFault = Sc_ZeroError << 1,
    //!Игнорирование тактовых импульсов
  Sc_ClockIgnore = Sc_SegFault << 1,
    //!Указана неверная команда
  Sc_IllInstr = Sc_ClockIgnore << 1,
  SC_Priv_Last = Sc_IllInstr
} Sc_FlagsReg;

//!инициализирует оперативную память и заполняет ее нулями
int sc_memoryInit();
//!задает значение ячейки памяти как value
int sc_memorySet(int addr, int val);
//!возвращает значение ячейки памяти в value
int sc_memoryGet(int addr, int* retVal);
//!созраняет содержимое памяти в бинарном виде
int sc_memorySave(char* filename);
//! загружает память из указанного файла
int sc_memoryLoad(char* filename);
//! инициализирует регистр флагов нулевыми значениями
int sc_regInit();
//! устанавливает значение регистра флагов
int sc_regSet(int reg, int val);
//! возвращает значения флага из регистра флагов
int sc_regGet(int reg, int* retVal);
//! выводит значение регистра флагов
void sc_printFlagReg();

//! Ввод аккумулятора
int sc_accumSet(int16_t val);
//! Вывод аккумулятора
int16_t sc_accumGet();

//! Ввод счетчика команд
int sc_counterSet(uint16 val);
//! Вывод счетчика команд
uint16 sc_counterGet();

#endif
