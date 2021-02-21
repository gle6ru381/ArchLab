#ifndef TERM_H
#define TERM_H

enum Colors {
    Color_1,
    Color_2,
    Color_3,
    Color_4,
    Color_5,
    Color_6,
    Color_7,
    Color_8
};

//! производит очистку и перемещение курсора
//! в левый верхний угол экрана
int mt_clrcr();

//! Перемещает курсор в указанную позицию. Первый
//! параметр номер строки, второй - номер столбца
int mt_gotoXY(int row, int column);

//! Определяет размер экрана терминала
int mt_getscreensize(int* rows, int* cols);

//! Устанавливает цвет последующих выводимых символов
int mt_setfgcolor(enum Colors color);

//! Устанавливает цвет фона песледующих выводимых символов
int mt_setbgcolor(enum Colors color);
#endif // TERM_H
