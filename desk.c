#include <stdio.h>
#include <stdlib.h>

#include "snake.h"

desk_t *initDesk(int width, int height) {
    desk_t *desk = (desk_t *)malloc(sizeof(desk_t));
    desk->width = width;
    desk->height = height;
    desk->field = (cell_t*)malloc(sizeof(cell_t) * width * height);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            desk->field[i*width + j].x = j;
            desk->field[i*width + j].y = i;
            desk->field[i*width + j].content = 'e';
        }
    }
    return desk;
}

int obstacleBeforeSnake(snake_t *snake, desk_t *desk) {
    int ret = 0;
    if (
        snake->direction == UP &&
        snake->snake_skeleton[0].y == (desk->startY + 1)
    ) {
        ret = 1;
        return ret;
    }
    else if (
        snake->direction == DOWN &&
        snake->snake_skeleton[0].y == (desk->endY - 1)
    ) {
        ret = 1;
        return ret;
    }
    else if (
        snake->direction == LEFT &&
        snake->snake_skeleton[0].x == (desk->startX + 1)
    ) {
        ret = 1;
        return ret;
    }
    else if (
        snake->direction == RIGHT &&
        snake->snake_skeleton[0].x == (desk->endX - 1)
    ) {
        ret = 1;
        return ret;
    }
    for (int k = 0; k < snake->length; k++) {
        if (
            snake->direction == UP                                                          &&
            snake->snake_skeleton[0].y == (snake->snake_skeleton[k].y + 1)  &&
            snake->snake_skeleton[0].x == snake->snake_skeleton[k].x
        ) {
            ret = 1;
            return ret;
        }
        else if (
            snake->direction == DOWN                                                        &&
            snake->snake_skeleton[0].y == (snake->snake_skeleton[k].y - 1)  &&
            snake->snake_skeleton[0].x == snake->snake_skeleton[k].x
        ) {
            ret = 1;
            return ret;
        }
        else if (
            snake->direction == LEFT                                                        &&
            snake->snake_skeleton[0].x == (snake->snake_skeleton[k].x + 1)  &&
            snake->snake_skeleton[0].y == snake->snake_skeleton[k].y  
        ) {
            ret = 1;
            return ret;
        }
        else if (
            snake->direction == RIGHT                                                       &&
            snake->snake_skeleton[0].x == (snake->snake_skeleton[k].x - 1)  &&
            snake->snake_skeleton[0].y == snake->snake_skeleton[k].y  
        ) {
            ret = 1;
            return ret;
        }
    }
    return ret;
}

int obstacleLeftOfSnake(snake_t *snake, desk_t *desk) {
    if (
        snake->direction == UP &&
        snake->snake_skeleton[0].x == (desk->startX + 1)
    )   return 1;
    else if (
        snake->direction == DOWN &&
        snake->snake_skeleton[0].x == (desk->endX - 1)
    )   return 1;
    else if (
        snake->direction == LEFT &&
        snake->snake_skeleton[0].y == (desk->endY - 1)
    )   return 1;
    else if (
        snake->direction == RIGHT &&
        snake->snake_skeleton[0].y == (desk->startY + 1)
    )   return 1;
    
    return 0;
}

int obstacleRightOfSnake(snake_t *snake, desk_t *desk) {
    if (
        snake->direction == UP &&
        snake->snake_skeleton[0].x == (desk->endX - 1)
    )   return 1;
    else if (
        snake->direction == DOWN &&
        snake->snake_skeleton[0].x == (desk->startX + 1)
    )   return 1;
    else if (
        snake->direction == LEFT &&
        snake->snake_skeleton[0].y == (desk->startY + 1)
    )   return 1;
    else if (
        snake->direction == RIGHT &&
        snake->snake_skeleton[0].y == (desk->endY - 1)
    )   return 1;
    return 0;
}

int snakeLeftOfSnake(snake_t *snake) {
    for (int k = 0; k < snake->length; k++) {
        if (
            snake->direction == UP                                                          &&
            snake->snake_skeleton[0].x == (snake->snake_skeleton[k].x + 1)  &&
            snake->snake_skeleton[0].y == snake->snake_skeleton[k].y
        )   return 1;
        else if (
            snake->direction == DOWN                                                        &&
            snake->snake_skeleton[0].x == (snake->snake_skeleton[k].x - 1)  &&
            snake->snake_skeleton[0].y == snake->snake_skeleton[k].y
        )   return 1;
        else if (
            snake->direction == LEFT                                                        &&
            snake->snake_skeleton[0].y == (snake->snake_skeleton[k].y - 1)  &&
            snake->snake_skeleton[0].x == snake->snake_skeleton[k].x
        )   return 1;
        else if (
            snake->direction == RIGHT                                                       &&
            snake->snake_skeleton[0].y == (snake->snake_skeleton[k].y + 1)  &&
            snake->snake_skeleton[0].x == snake->snake_skeleton[k].x
        )   return 1;
    }
    return 0;
}

int snakeRightOfSnake(snake_t *snake) {
    for (int k = 0; k < snake->length; k++) {
        if (
            snake->direction == UP                                                          &&
            snake->snake_skeleton[0].x == (snake->snake_skeleton[k].x - 1)  &&
            snake->snake_skeleton[0].y == snake->snake_skeleton[k].y
        )   return 1;
        else if (
            snake->direction == DOWN                                                        &&
            snake->snake_skeleton[0].x == (snake->snake_skeleton[k].x + 1)  &&
            snake->snake_skeleton[0].y == snake->snake_skeleton[k].y
        )   return 1;
        else if (
            snake->direction == LEFT                                                        &&
            snake->snake_skeleton[0].y == (snake->snake_skeleton[k].y + 1)  &&
            snake->snake_skeleton[0].x == snake->snake_skeleton[k].x
        )   return 1;
        else if (
            snake->direction == RIGHT                                                       &&
            snake->snake_skeleton[0].y == (snake->snake_skeleton[k].y - 1)  &&
            snake->snake_skeleton[0].x == snake->snake_skeleton[k].x
        )   return 1;
    }
    return 0;
}

int checkFieldOccupation(snake_t *snake, desk_t *desk) {
    int freeLeft = snake->max_length, freeRight = snake->max_length;
    for (int k = 1; k < snake->length; k++) {
        if (
            snake->direction == UP                                                      &&
            snake->snake_skeleton[k].x < snake->snake_skeleton[0].x     &&
            snake->snake_skeleton[k].y == snake->snake_skeleton[0].y    &&
            snake->snake_skeleton[0].x - snake->snake_skeleton[k].x < freeLeft
        )   freeLeft = snake->snake_skeleton[0].x - snake->snake_skeleton[k].x;
        if (
            snake->direction == UP                                                      &&
            snake->snake_skeleton[k].x > snake->snake_skeleton[0].x     &&
            snake->snake_skeleton[k].y == snake->snake_skeleton[0].y    &&
            snake->snake_skeleton[k].x - snake->snake_skeleton[0].x < freeRight
        )   freeRight = snake->snake_skeleton[k].x - snake->snake_skeleton[0].x;
        if (
            snake->direction == DOWN                                                    &&
            snake->snake_skeleton[k].x < snake->snake_skeleton[0].x     &&
            snake->snake_skeleton[k].y == snake->snake_skeleton[0].y    &&
            snake->snake_skeleton[0].x - snake->snake_skeleton[k].x < freeRight
        )   freeRight = snake->snake_skeleton[0].x - snake->snake_skeleton[k].x;
        if (
            snake->direction == DOWN                                                    &&
            snake->snake_skeleton[k].x > snake->snake_skeleton[0].x     &&
            snake->snake_skeleton[k].y == snake->snake_skeleton[0].y    &&
            snake->snake_skeleton[k].x - snake->snake_skeleton[0].x < freeLeft
        )   freeLeft = snake->snake_skeleton[k].x - snake->snake_skeleton[0].x;
        if (
            snake->direction == LEFT                                                    &&
            snake->snake_skeleton[k].y < snake->snake_skeleton[0].y     &&
            snake->snake_skeleton[k].x == snake->snake_skeleton[0].x    &&
            snake->snake_skeleton[0].y - snake->snake_skeleton[k].y < freeRight
        )   freeRight = snake->snake_skeleton[0].y - snake->snake_skeleton[k].y;
        if (
            snake->direction == LEFT                                                    &&
            snake->snake_skeleton[k].y > snake->snake_skeleton[0].y     &&
            snake->snake_skeleton[k].x == snake->snake_skeleton[0].x    &&
            snake->snake_skeleton[k].y - snake->snake_skeleton[0].y < freeLeft
        )   freeLeft = snake->snake_skeleton[k].y - snake->snake_skeleton[0].y;
        if (
            snake->direction == RIGHT                                                   &&
            snake->snake_skeleton[k].y < snake->snake_skeleton[0].y     &&
            snake->snake_skeleton[k].x == snake->snake_skeleton[0].x    &&
            snake->snake_skeleton[0].y - snake->snake_skeleton[k].y < freeLeft
        )   freeLeft = snake->snake_skeleton[0].y - snake->snake_skeleton[k].y;
        if (
            snake->direction == RIGHT                                                   &&
            snake->snake_skeleton[k].y > snake->snake_skeleton[0].y     &&
            snake->snake_skeleton[k].x == snake->snake_skeleton[0].x    &&
            snake->snake_skeleton[k].y - snake->snake_skeleton[0].y < freeRight
        )   freeRight = snake->snake_skeleton[k].y - snake->snake_skeleton[0].y;
    }
    if (freeLeft < freeRight) return RIGHT;
    else return LEFT;
}

void freeDesk(desk_t *desk) {
    free(desk);
    desk = NULL;
}