#include "snake.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

void moveSnakeManualTwoSnakes(snake_t *snake, snake_t *secondSnake, cell_t *food, desk_t *desk) {
    //inputs part-------
    char ch;
    int r = read(0, &ch, 1);

    int lastCoords[2] = {
        snake->snake_skeleton[snake->length].x,
        snake->snake_skeleton[snake->length].y, 
    };

    for (int k = snake->length; k > 0; k--) {
        snake->snake_skeleton[k].x = snake->snake_skeleton[k - 1].x;
        snake->snake_skeleton[k].y = snake->snake_skeleton[k - 1].y;
    }

    if (r==1)
    {   
        if (ch == 'a') {
          snakeTurnLeft(snake);
        }
        else if (ch == 'd') {
          snakeTurnRight(snake);
        }
    }

    //logic part--------
    snakeStep(snake, desk);
    if (
        checkWallsCollisions(snake, desk)   ||
        checkItselfCollisions(snake)        ||
        checkOtherSnakeCollisions(snake, secondSnake)
    ) {
        // printf("Gameover!\n");
        snake->gameOver = true;
    }

    snakeEats(food, snake, secondSnake, desk, lastCoords[0], lastCoords[1]);
}

void moveSnakeAITwoSnakes(snake_t *snake, snake_t *secondSnake, cell_t *food, desk_t *desk) {
    //inputs part-------
    int lastCoords[2] = {
        snake->snake_skeleton[snake->length].x,
        snake->snake_skeleton[snake->length].y, 
    };
    for (int k = snake->length; k > 0; k--) {
        snake->snake_skeleton[k].x = snake->snake_skeleton[k - 1].x;
        snake->snake_skeleton[k].y = snake->snake_skeleton[k - 1].y;
    }
    snakeControllerTwoSnakes(snake, secondSnake, desk, food);

    //logic part--------
    snakeStep(snake, desk);
    if (
        checkWallsCollisions(snake, desk)   ||
        checkItselfCollisions(snake)        ||
        checkOtherSnakeCollisions(snake, secondSnake)
    ) {
        // printf("Gameover!\n");
        snake->gameOver = true;
    }
    snakeEats(food, snake, secondSnake, desk, lastCoords[0], lastCoords[1]);
}

int checkWallsCollisions(snake_t *snake, desk_t *desk) {
    if (
        snake->snake_skeleton[0].x == 1  ||
        snake->snake_skeleton[0].x == desk->width ||
        snake->snake_skeleton[0].y == 1  ||
        snake->snake_skeleton[0].y == desk->height
    )   return 1;
    return 0;
}

int checkItselfCollisions(snake_t *snake) {
    for (int k = 1; k < snake->length ; k++) {
        if (
            snake->snake_skeleton[k].x == snake->snake_skeleton[0].x &&
            snake->snake_skeleton[k].y == snake->snake_skeleton[0].y 
        )   return 1;
    }
    return 0;
}

int checkOtherSnakeCollisions(snake_t *snake, snake_t *secondSnake) {
    for (int k = 0; k < secondSnake->length ; k++) {
        if (
            secondSnake->snake_skeleton[k].x == snake->snake_skeleton[0].x &&
            secondSnake->snake_skeleton[k].y == snake->snake_skeleton[0].y 
        )   return 1;
    }
    return 0;
}

int obstacleBeforeSnakeTwoSnakes(snake_t *snake, snake_t *otherSnake, desk_t *desk) {
    int ret = 0;
    if (
        snake->direction == UP &&
        snake->snake_skeleton[0].y == 2
    ) {
        ret = 1;
        return ret;
    }
    else if (
        snake->direction == DOWN &&
        snake->snake_skeleton[0].y == desk->height - 1
    ) {
        ret = 1;
        return ret;
    }
    else if (
        snake->direction == LEFT &&
        snake->snake_skeleton[0].x == 2
    ) {
        ret = 1;
        return ret;
    }
    else if (
        snake->direction == RIGHT &&
        snake->snake_skeleton[0].x == desk->width - 1
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
    for (int k = 0; k < snake->length; k++) {
        if (
            snake->direction == UP                                                          &&
            snake->snake_skeleton[0].y == (otherSnake->snake_skeleton[k].y + 1)  &&
            snake->snake_skeleton[0].x == otherSnake->snake_skeleton[k].x    
        ) {
            ret = 1;
            return ret;
        }
        else if (
            snake->direction == DOWN                                                        &&
            snake->snake_skeleton[0].y == (otherSnake->snake_skeleton[k].y - 1)  &&
            snake->snake_skeleton[0].x == otherSnake->snake_skeleton[k].x
        ) {
            ret = 1;
            return ret;
        }
        else if (
            snake->direction == LEFT                                                        &&
            snake->snake_skeleton[0].x == (otherSnake->snake_skeleton[k].x + 1)  &&
            snake->snake_skeleton[0].y == otherSnake->snake_skeleton[k].y  
        ) {
            ret = 1;
            return ret;
        }
        else if (
            snake->direction == RIGHT                                                       &&
            snake->snake_skeleton[0].x == (otherSnake->snake_skeleton[k].x - 1)  &&
            snake->snake_skeleton[0].y == otherSnake->snake_skeleton[k].y  
        ) {
            ret = 1;
            return ret;
        }
    }
    return ret;
}

int snakeLeftOfSnakeTwoSnakes(snake_t *snake, snake_t *otherSnake) {
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
    for (int k = 0; k < snake->length; k++) {
        if (
            snake->direction == UP                                                          &&
            snake->snake_skeleton[0].x == (otherSnake->snake_skeleton[k].x + 1)  &&
            snake->snake_skeleton[0].y == otherSnake->snake_skeleton[k].y
        )   return 1;
        else if (
            snake->direction == DOWN                                                        &&
            snake->snake_skeleton[0].x == (otherSnake->snake_skeleton[k].x - 1)  &&
            snake->snake_skeleton[0].y == otherSnake->snake_skeleton[k].y
        )   return 1;
        else if (
            snake->direction == LEFT                                                        &&
            snake->snake_skeleton[0].y == (otherSnake->snake_skeleton[k].y - 1)  &&
            snake->snake_skeleton[0].x == otherSnake->snake_skeleton[k].x
        )   return 1;
        else if (
            snake->direction == RIGHT                                                       &&
            snake->snake_skeleton[0].y == (otherSnake->snake_skeleton[k].y + 1)  &&
            snake->snake_skeleton[0].x == otherSnake->snake_skeleton[k].x
        )   return 1;
    }
    return 0;
}

int snakeRightOfSnakeTwoSnakes(snake_t *snake, snake_t *otherSnake) {
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
    for (int k = 0; k < snake->length; k++) {
        if (
            snake->direction == UP                                                          &&
            snake->snake_skeleton[0].x == (otherSnake->snake_skeleton[k].x - 1)  &&
            snake->snake_skeleton[0].y == otherSnake->snake_skeleton[k].y
        )   return 1;
        else if (
            snake->direction == DOWN                                                        &&
            snake->snake_skeleton[0].x == (otherSnake->snake_skeleton[k].x + 1)  &&
            snake->snake_skeleton[0].y == otherSnake->snake_skeleton[k].y
        )   return 1;
        else if (
            snake->direction == LEFT                                                        &&
            snake->snake_skeleton[0].y == (otherSnake->snake_skeleton[k].y + 1)  &&
            snake->snake_skeleton[0].x == otherSnake->snake_skeleton[k].x
        )   return 1;
        else if (
            snake->direction == RIGHT                                                       &&
            snake->snake_skeleton[0].y == (otherSnake->snake_skeleton[k].y - 1)  &&
            snake->snake_skeleton[0].x == otherSnake->snake_skeleton[k].x
        )   return 1;
    }
    return 0;
}

void snakeControllerTwoSnakes(snake_t *snake, snake_t *otherSnake, desk_t *desk, cell_t *food) {
    int dx, dy;
    dx = food->x - snake->snake_skeleton[0].x;
    dy = food->y - snake->snake_skeleton[0].y;

    if (obstacleBeforeSnakeTwoSnakes(snake, otherSnake, desk) && obstacleLeftOfSnake(snake, desk)) {
        snakeTurnRight(snake);
    }
    else if (obstacleBeforeSnakeTwoSnakes(snake, otherSnake, desk) && obstacleRightOfSnake(snake, desk)) {
        snakeTurnLeft(snake);
    }
    else if (obstacleBeforeSnakeTwoSnakes(snake, otherSnake, desk)) {
        if (checkFieldOccupation(snake, desk) == LEFT) snakeTurnLeft(snake);
        else if (checkFieldOccupation(snake, desk) == RIGHT) snakeTurnRight(snake);
    }
    else if (snakeLeftOfSnakeTwoSnakes(snake, otherSnake));
    else if (snakeRightOfSnakeTwoSnakes(snake, otherSnake));
    else {
        switch (snake->direction) {
            case LEFT:
                if (dy < 0 && dx == 0) snakeTurnRight(snake);
                else if (dy > 0 && dx == 0) snakeTurnLeft(snake);
                break;
            case RIGHT:
                if (dy > 0 && dx == 0) snakeTurnRight(snake);
                else if (dy < 0 && dx == 0) snakeTurnLeft(snake);
                break;
            case UP:
                if (dy == 0 && dx > 0) snakeTurnRight(snake);
                else if (dy == 0 && dx < 0) snakeTurnLeft(snake);
                break;
            case DOWN:
                if (dy == 0 && dx < 0) snakeTurnRight(snake);
                else if (dy == 0 && dx > 0) snakeTurnLeft(snake);
                break;
        }
    }
}