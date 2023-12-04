#include "obstacles.h"

#include "raylib.h"
#include <stdlib.h>

#define MAX_OBSTACLES 255
Obstacle _obstacles[MAX_OBSTACLES] = {0};

void AddObstacle(Obstacle obstacle)
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (_obstacles[i].active) continue;
        _obstacles[i] = (Obstacle){obstacle.hitbox, obstacle.color, 1};
        break;
    }
}

void DrawObstacles()
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (!_obstacles[i].active) continue;
        Obstacle obstacle = _obstacles[i];
        Rectangle rect = obstacle.hitbox;
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, obstacle.color);
    }
}

int CheckObstacleCollision(Rectangle rect)
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (!_obstacles[i].active) continue;
        Rectangle obstacle = _obstacles[i].hitbox;
        if (CheckCollisionRecs(rect, obstacle)) return 1;
    }
    return 0;
}