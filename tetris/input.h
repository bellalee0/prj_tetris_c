#ifndef INPUT_H
#define INPUT_H

typedef enum {
    KEY_NONE,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ROTATE,
    KEY_HARD_DROP,
    KEY_QUIT
} GameKey;

GameKey GetKey(void);

#endif
