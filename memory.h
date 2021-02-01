#include <string.h>

typedef union {
  char buff;
  char overflow : 1;
  char zeroError : 1;
  char segFault : 1;
  char clockIgnore : 1;
  char illInstr : 1;
} flagReg;

int sc_memoryInit();
int sc_memorySet(int addr, int val);
int sc_memotyGet(int addr, int* retVal);
int sc_memotySave(char* filename);
int sc_memoryLoad(char* filename);
int sc_regInit();
int sc_regSet(int reg, int* val);
int sc_regGete(int reg, int* retVal);
