
#include <stdbool.h>

//Cell data type
typedef struct {
    int x;
    int y;
} cell_t;

//Snake data type
typedef struct {
    int length;
    int max_length;
    int score;
    int direction;
    char turnLeft;
    char turnRight;
    bool gameOver;
    cell_t *snake_skeleton;
} snake_t;

//Game field data type
typedef struct {
    int width;
    int height;
    int startX;
    int startY;
    int endX;
    int endY;
    cell_t *field;
} desk_t;