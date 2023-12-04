#ifndef OBSTACLES_H_
#define OBSTACLES_H_

#include "raylib.h"

typedef struct {
    Rectangle hitbox;
    Color color;
    int active;
} Obstacle;

void AddObstacle(Obstacle obstacle);
void DrawObstacles(void);
int CheckObstacleCollision(Rectangle rect);

#endif