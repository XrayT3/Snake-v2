#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "snake.h"

cell_t *initFood(int coordX, int coordY) {
    cell_t *food = (cell_t *)malloc(sizeof(cell_t));
    food->x = coordX;
    food->y = coordY;
    return food;
}

void freeFood(cell_t *food) {
    free(food);
    food = NULL;
}

void updateFood(desk_t *desk, cell_t *food, snake_t *snake, snake_t *secondSnake) {
    int newX, newY;
    int maxX = desk->width, minX = 1 + 1;
    int maxY = desk->height, minY = 1 + 1;
    srand(time(NULL));
    calculate:
    newX = rand() % (maxX - minX) + minX;
    newY = rand() % (maxY - minY) + minY;

    for (int k = 0; k < snake->length; k++) {
        if (
            newX == snake->snake_skeleton[k].x &&
            newY == snake->snake_skeleton[k].y
        )   goto calculate;
    }
    if(secondSnake != NULL)
        for (int k = 0; k < secondSnake->length; k++) {
            if (
                newX == secondSnake->snake_skeleton[k].x &&
                newY == secondSnake->snake_skeleton[k].y
            )   goto calculate;
        }

    // printf("New coords: %d %d\n", newX, newY);
    food->x = newX;
    food->y = newY;
}