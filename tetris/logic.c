#include "logic.h"
#include "game.h"

#include <stdlib.h>

ActiveBlock SpawnTetromino(void) {

    int block_type = rand() % 7;

    Tetromino tetromino;

    switch (block_type) {
        case 0: tetromino = BLOCK_I; break;
        case 1: tetromino = BLOCK_O; break;
        case 2: tetromino = BLOCK_T; break;
        case 3: tetromino = BLOCK_J; break;
        case 4: tetromino = BLOCK_L; break;
        case 5: tetromino = BLOCK_S; break;
        case 6: tetromino = BLOCK_Z; break;
    }

    ActiveBlock activeBlock = {
        .tetromino = tetromino,
        .x = BOARD_ORIGIN_X + 1 + (BOARD_COLS - 4) / 2 * 2,
        .y = BOARD_ORIGIN_Y
    };

    return activeBlock;
}

ActiveBlock MoveTetromino(ActiveBlock activeBlock, int dx, int dy) {
    
    activeBlock.x = dx;
    activeBlock.y = dy;

    return activeBlock;
}

ActiveBlock RotateTetromino(ActiveBlock activeBlock) {

    ActiveBlock newBlock = activeBlock;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            newBlock.tetromino.shape[i][j] = activeBlock.tetromino.shape[4 - j - 1][i];
        }
    }

    return newBlock;
}