#ifndef BIGCHARS_H
#define BIGCHARS_H
#include "term.h"

#define BIT(n) (1u << (n))

//! Выводит строку символов с использованием
//! дополнительной кодировочной таблицы
int bc_printA(char* str);

//! Выводит на экран псевдографическую рамку, с верхним
//! левым углом row,column, шириной width и высотой height
int bc_box(int row, int column, int width, int height);

//! Выводит на экран большой символ размером 8x8,
//! с верхним углом в row,column с фоном colorbg цвета colorfg
int bc_printbigchar(int symbol[2], int row, int column,
    enum Colors colorbg, enum Colors colorfg);

//! устанавливает значение row,column символа big в val
int bc_setbigcharpos(int* big, int row, int column, int val);

//! возвращает значение row,column символа big в val
int bc_getbigcharpos(int* big, int row, int column, int* val);

//! записывает заданое число больших символов в файл fd
int bc_bigcharwrite(int fd, int* big, int count);

//! считывает из файла заданное количество больших символов
int bc_bigcharread(int fd, int* big, int need_count, int* count);
#endif // BIGCHARS_H
