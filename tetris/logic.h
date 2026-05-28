#ifndef LOGIC_H
#define LOGIC_H

#include "block.h"

ActiveBlock SpawnTetromino(void);
ActiveBlock MoveTetromino(ActiveBlock activeBlock, int dx, int dy);
ActiveBlock RotateTetromino(ActiveBlock activeBlock);
ActiveBlock HardDrop(ActiveBlock activeBlock);

#endif
