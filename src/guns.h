#ifndef GUNS_H_
#define GUNS_H_

#include "raylib.h"
#include "player.h"

typedef struct {
    Rectangle hitbox;
    Vector2 velocity;
    int active;
} Bullet;

void DrawGun(Player player);
void UpdateGun(Player *player);

#endif