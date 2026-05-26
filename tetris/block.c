#include "block.h"
#include <stdio.h>

Tetromino BLOCK_I = {
	.shape = {
		0,0,0,0,
		2,2,2,2,
		0,0,0,0,
		0,0,0,0
	},
	.color = SKYBLUE
};

Tetromino BLOCK_O = {
	.shape = {
		0,0,0,0,
		0,2,2,0,
		0,2,2,0,
		0,0,0,0
	},
	.color = YELLOW
};

Tetromino BLOCK_T = {
	.shape = {
		0,0,0,0,
		0,2,2,2,
		0,0,2,0,
		0,0,0,0
	},
	.color = PURPLE
};

Tetromino BLOCK_J = {
	.shape = {
		0,0,0,0,
		2,2,2,0,
		0,0,2,0,
		0,0,0,0
	},
	.color = DARKBLUE
};

Tetromino BLOCK_L = {
	.shape = {
		0,0,0,0,
		0,2,2,2,
		0,2,0,0,
		0,0,0,0
	},
	.color = DARKYELLOW
};

Tetromino BLOCK_S = {
	.shape = {
		0,0,0,0,
		0,0,2,2,
		0,2,2,0,
		0,0,0,0
	},
	.color = GREEN
};

Tetromino BLOCK_Z = {
	.shape = {
		0,0,0,0,
		0,2,2,0,
		0,0,2,2,
		0,0,0,0
	},
	.color = RED
};