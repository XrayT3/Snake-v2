#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "snake.h"

food_t *initFood(int coordX, int coordY) {
    food_t *food = (food_t *)malloc(sizeof(food_t));
    food->x = coordX;
    food->y = coordY;
    return food;
}

void freeFood(food_t *food) {
    free(food);
    food = NULL;
}

void updateFood(desk_t *desk, food_t *food, snake_t *snake) {
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

    // printf("New coords: %d %d\n", newX, newY);
    food->x = newX;
    food->y = newY;
}