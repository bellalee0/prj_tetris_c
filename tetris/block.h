#ifndef BLOCK_H
#define BLOCK_H

#include "console.h"

typedef struct {
	int shape[4][4];
	Color color;
} Tetromino;

extern Tetromino BLOCK_I;
extern Tetromino BLOCK_O;
extern Tetromino BLOCK_T;
extern Tetromino BLOCK_J;
extern Tetromino BLOCK_L;
extern Tetromino BLOCK_S;
extern Tetromino BLOCK_Z;

typedef struct {
	Tetromino tetromino;
	int x;
	int y;
} ActiveBlock;

#endif