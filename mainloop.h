#ifndef MAINLOOP_H
#define MAINLOOP_H

int loop_exec();

void drawMemory();
void drawAccumulator();
void drawInstructionCounter(int);
void drawOperationWin();
void drawFlagsWin();
void drawKeys();
void drawBigCell();
void fillContext();
void fillRow(int row);

#endif // MAINLOOP_H
