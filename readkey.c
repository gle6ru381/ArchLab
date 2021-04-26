#include "readkey.h"
#include "memory.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static enum Keys receiveEscSeq()
{
    unsigned char sym;

    enum Keys retval;

    sym = getchar();
    if (sym != 0x5B)
        return KEY_Illegal;
    sym = getchar();
    switch (sym) {
    case 0x41:
        retval = KEY_up;
        return retval;
    case 0x42:
        retval = KEY_down;
        return retval;
    case 0x43:
        retval = KEY_right;
        return retval;
    case 0x44:
        retval = KEY_left;
        return retval;
    case 0x31:
        break;
    default:
        return KEY_Illegal;
    }

    sym = getchar();
    if (sym == 0x35)
        retval = KEY_F5;
    else if (sym == 0x37)
        retval = KEY_F6;
    else
        return KEY_Illegal;
    sym = getchar();
    if (sym != 0x7E)
        return KEY_Illegal;

    return retval;
}

int rk_readKey(enum Keys* key)
{
    unsigned char sym = getchar();

//    sc_regSet(Sc_ClockIgnore, 1);

    if (sym == 0x1B) {
        *key = receiveEscSeq();
        if (*key == KEY_Illegal)
            return -1;
    } else {
        switch (sym) {
        case 'l':
            *key = KEY_l;
            break;
        case 's':
            *key = KEY_s;
            break;
        case 'r':
            *key = KEY_r;
            break;
        case 't':
            *key = KEY_t;
            break;
        case 'i':
            *key = KEY_i;
            break;
        case '\n':
            *key = KEY_ENTER;
            break;
        default:
            *key = KEY_Illegal;
            return -1;
        }
    }
//    sc_regSet(Sc_ClockIgnore, 0);
    return 0;
}

int rk_mytermsave()
{
    sc_regSet(Sc_ClockIgnore, 1);
    rk_mytermregime(0, 0, 0, 1, 0);
    printf("Введите имя файла:\n");
    char fileName[250];
    scanf("%s", fileName);
    rk_mytermregime(1, 0, 0, 0, 0);
    sc_regSet(Sc_ClockIgnore, 0);

    return sc_memorySave(fileName);
}

int rk_mytermrestore()
{
    sc_regSet(Sc_ClockIgnore, 1);
    rk_mytermregime(0, 0, 0, 1, 0);
    printf("Введите имя файла:\n");
    char fileName[250];
    scanf("%s", fileName);
    rk_mytermregime(1, 0, 0, 0, 0);
    sc_regSet(Sc_ClockIgnore, 0);

    return sc_memoryLoad(fileName);
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    struct termios term;
    tcgetattr(0, &term);
    if (regime) {
        term.c_lflag &= ~ICANON;
    } else {
        term.c_lflag |= ICANON;
    }

    if (echo) {
        term.c_lflag |= ECHO;
    } else {
        term.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &term);
    return 0;
}
