#ifndef TERM_H
#define TERM_H

enum Colors {
    Color_Black,
    Color_Red,
    Color_Green,
    Color_Orange,
    Color_Blue,
    Color_Purple,
    Color_Turq,
    Color_White
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
