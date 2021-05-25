
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#include "snake.h"

snake_t *initSnake(int displayWidth, int displayHeight, int initSnakeX, int initSnakeY, char turnLeft, char turnRight, desk_t* desk) {

    snake_t *snake = (snake_t *)malloc(sizeof(snake_t));
    snake->length = 2;
    snake->max_length = displayWidth * displayHeight;
    snake->score = 0;
    snake->direction = LEFT;
    snake->turnLeft = turnLeft;
    snake->turnRight = turnRight;
    snake->gameOver = false;
    snake->snake_skeleton = (cell_t*)malloc(sizeof(cell_t) * snake->max_length);
    for (int i = 0; i < 2; i++) {
        snake->snake_skeleton[i].x = initSnakeX + i;
        snake->snake_skeleton[i].y = initSnakeY;
    }
    return snake;
}

snake_t *initSnakeAI(int displayWidth, int displayHeight, int initSnakeX, int initSnakeY, desk_t* desk) {

    snake_t *snake = (snake_t *)malloc(sizeof(snake_t));
    snake->length = 2;
    snake->max_length = displayWidth * displayHeight;
    snake->score = 0;
    snake->direction = LEFT;
    snake->gameOver = false;
    snake->snake_skeleton = (cell_t *)malloc(sizeof(cell_t) * snake->max_length);
    for (int i = 0; i < 2; i++) {
        snake->snake_skeleton[i].x = initSnakeX + i;
        snake->snake_skeleton[i].y = initSnakeY;
    }
    return snake;
}

void increaseSnake(snake_t *snake, int x, int y, desk_t* desk) {
    snake->length += 1;
    snake->snake_skeleton[snake->length].x = x;
    snake->snake_skeleton[snake->length].y = y;
}

void moveSnakeManual(snake_t *snake, cell_t *food, desk_t *desk){
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
        if (ch == snake->turnLeft) {
          snakeTurnLeft(snake);
        }
        else if (ch == snake->turnRight) {
          snakeTurnRight(snake);
        }
    }

    //logic part--------
    snakeStep(snake, desk);
    if (
        checkCollisions(snake, desk)      
    ) {
        // printf("Gameover!\n");
        snake->gameOver = true;
    }

    snakeEats(food, snake, NULL, desk, lastCoords[0], lastCoords[1]);

}

void moveSnakeAI(snake_t *snake, cell_t *food, desk_t *desk) {

    //inputs part-------
    int lastCoords[2] = {
        snake->snake_skeleton[snake->length].x,
        snake->snake_skeleton[snake->length].y, 
    };

    for (int k = snake->length; k > 0; k--) {
        snake->snake_skeleton[k].x = snake->snake_skeleton[k - 1].x;
        snake->snake_skeleton[k].y = snake->snake_skeleton[k - 1].y;
    }

    snakeController(snake, desk, food);

    //logic part--------
    snakeStep(snake, desk);

    if (
        checkWallsCollisions(snake, desk)   ||
        checkItselfCollisions(snake)        
    ) {
        // printf("Gameover!\n");
        snake->gameOver = true;
        //change to gameover menu
    }

    snakeEats(food, snake, NULL, desk, lastCoords[0], lastCoords[1]);
}

void snakeStep(snake_t *snake, desk_t *desk) {

    switch (snake->direction) {
        case LEFT:
            snake->snake_skeleton[0].x -= 1;
            break;
        case RIGHT:
            snake->snake_skeleton[0].x += 1;
            break;
        case UP:
            snake->snake_skeleton[0].y -= 1;
            break;
        case DOWN:
            snake->snake_skeleton[0].y += 1;
            break;
    }
}

bool checkCollisions(snake_t *snake, desk_t *desk) {
    bool ret = false;
    for (int k = 1; k < snake->length ; k++) {
        if (
            snake->snake_skeleton[k].x == snake->snake_skeleton[0].x &&
            snake->snake_skeleton[k].y == snake->snake_skeleton[0].y 

        ) {
            ret = true;
            break;
        }
    }
    if (
        snake->snake_skeleton[0].x == 1  ||
        snake->snake_skeleton[0].x == desk->width ||
        snake->snake_skeleton[0].y == 1  ||
        snake->snake_skeleton[0].y == desk->height
    ) {
        ret = true;
    }
    return ret;
}

void snakeController(snake_t *snake, desk_t *desk, cell_t *food) {

    int dx, dy;

    dx = food->x - snake->snake_skeleton[0].x;
    dy = food->y - snake->snake_skeleton[0].y;

    // printf("Snake actual direction: %d\n\r", snake->direction);

    if (obstacleBeforeSnake(snake, desk) && obstacleLeftOfSnake(snake, desk)) {
        snakeTurnRight(snake);
        // printf("Obstacle front and left\n\r");
    }
    else if (obstacleBeforeSnake(snake, desk) && obstacleRightOfSnake(snake, desk)) {
        snakeTurnLeft(snake);
        // printf("Obstacle front and right\n\r");
    }
    else if (obstacleBeforeSnake(snake, desk)) {
        if (checkFieldOccupation(snake, desk) == LEFT) snakeTurnLeft(snake);
        else if (checkFieldOccupation(snake, desk) == RIGHT) snakeTurnRight(snake);
    }
    else if (snakeLeftOfSnake(snake));
    else if (snakeRightOfSnake(snake));
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

void snakeEats(cell_t *food, snake_t *snake, snake_t *secondSnake, desk_t *desk, int lastCoordX, int lastCoordY) {
    if (
        snake->snake_skeleton[0].x == food->x &&
        snake->snake_skeleton[0].y == food->y
    ) {
        snake->score += 1;
        increaseSnake(snake, lastCoordX, lastCoordY, desk);
        updateFood(desk, food, snake, secondSnake);
    }
}

void snakeTurnLeft(snake_t *snake) {
    switch (snake->direction) {
        case LEFT:
            snake->direction = DOWN;
            break;
        case DOWN:
            snake->direction = RIGHT;
            break;              
        case RIGHT:
            snake->direction = UP;
            break;
        case UP:
            snake->direction = LEFT;
            break;
    }
}

void snakeTurnRight(snake_t *snake) {
    switch (snake->direction) {
        case LEFT:
            snake->direction = UP;
            break;
        case DOWN:
            snake->direction = LEFT;
            break;              
        case RIGHT:
            snake->direction = DOWN;
            break;
        case UP:
            snake->direction = RIGHT;
            break;
    }
}

int get_record(int a, int b, int c, int d){
    if (a < b) a = b;
    if (a < c) a = c;
    if (a < d) a = d;
    return a;
}

void freeSnake(snake_t *snake) {

    free(snake->snake_skeleton);
    snake->snake_skeleton = NULL;
    free(snake);
    snake = NULL;
}