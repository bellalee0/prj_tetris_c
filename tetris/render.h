#ifndef RENDER_H
#define RENDER_H

#include "block.h"

void DrawBorder(void);
void DrawBoard(void);
void DrawTetromino(ActiveBlock activeBlock);
void EraseTetromino(ActiveBlock activeBlock);

#endif
