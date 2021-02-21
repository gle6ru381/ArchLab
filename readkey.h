#ifndef READKEY_H
#define READKEY_H

enum Keys {
    KEY_l,
    KEY_s,
    KEY_F5,
    KEY_F6,
    KEY_r,
    KEY_t,
    KEY_i,
    KEY_Illegal,
    KEY_left,
    KEY_right,
    KEY_up,
    KEY_down
};

//! анализирует последовательность символов и возвращает
//! нажатую клавишу
int rk_readKey(enum Keys* key);

//! сохраняет текущие параметры терминала
int rk_mytermsave();

//! восстанавливает сохраненные параметры терминала
int rk_mytermrestore();

//! переключает терминал между режимами
int rk_mytermregime(int regime, int vtime, int vmin,
                    int echo, int sigint);

#endif // READKEY_H
