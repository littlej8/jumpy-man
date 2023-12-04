#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"

typedef struct
{
    Vector2 position, velocity, size;
    float max_speed, acceleration, friction;
    float jump_power, gravity;

    Vector2 gun_pos;
} Player;

void DrawPlayer(Player player);
void UpdatePlayer(Player *player);

#endif