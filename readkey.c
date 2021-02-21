#include "readkey.h"
#include "memory.h"
#include <stdio.h>

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

    if (sym == '\n') {
        *key = KEY_Illegal;
        return 0;
    }

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
        default:
            *key = KEY_Illegal;
            return -1;
        }
    }
    return 0;
}

int rk_mytermsave()
{
    printf("Введите имя файла:\n");
    char fileName[250];
    scanf("%s", fileName);

    return sc_memorySave(fileName);
}

int rk_mytermrestore()
{
    printf("Введите имя файла:\n");
    char fileName[250];
    scanf("%s", fileName);

    return sc_memoryLoad(fileName);
}
