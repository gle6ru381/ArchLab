#include "memory.h"
#define SC_MEMORY_SIZE 100
unsigned short memory[SC_MEMORY_SIZE];

int sc_memoryInit()
{
  memset(memory, 0, SC_MEMORY_SIZE);
}
