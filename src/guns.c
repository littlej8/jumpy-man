#include "guns.h"

#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "obstacles.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BULLETS 32
#define DEFAULT_BULLET_SPEED 50.f
Bullet _bullets[MAX_BULLETS] = {0};

void DrawBullets(Player player)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!_bullets[i].active) continue;
        Bullet bullet = _bullets[i];

        Vector2 next_pos = Vector2Add((Vector2){bullet.hitbox.x, bullet.hitbox.y}, bullet.velocity);
        float rotation = atan2(next_pos.y - bullet.hitbox.y, next_pos.x - bullet.hitbox.x) * RAD2DEG;
        DrawRectanglePro(bullet.hitbox, (Vector2){0, 0}, rotation, ORANGE);
    }
}

void DrawGun(Player player)
{
    float rotation = atan2(player.gun_pos.y - player.position.y, player.gun_pos.x - player.position.x) * RAD2DEG;
    Rectangle rect = {player.gun_pos.x, player.gun_pos.y, 15, 7.5};
    DrawRectanglePro(rect, (Vector2){0, 0}, rotation, BLACK);

    DrawBullets(player);
}

void ShootGun(Player *player)
{
    Vector2 velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(player->gun_pos, player->position)), DEFAULT_BULLET_SPEED);
    Rectangle rect = {player->gun_pos.x, player->gun_pos.y, 15, 7.5};
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (_bullets[i].active) continue;
        _bullets[i] = (Bullet){rect, velocity, 1};
        break;
    }
}

int OutOfBounds(Bullet bullet)
{
    Rectangle box = bullet.hitbox;
    if (box.x < 0 ||
        box.x > GetScreenWidth() ||
        box.y < 0 ||
        box.y > GetScreenHeight())
        return 1;
    return 0;
}

void UpdateBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!_bullets[i].active) continue;
        Bullet bullet = _bullets[i];

        Vector2 next_pos = Vector2Add((Vector2){bullet.hitbox.x, bullet.hitbox.y}, bullet.velocity);
        _bullets[i].hitbox = (Rectangle){next_pos.x, next_pos.y, bullet.hitbox.width, bullet.hitbox.height};

        if (CheckObstacleCollision(_bullets[i].hitbox) || OutOfBounds(_bullets[i]))
        {
            _bullets[i].active = 0;
        }
    }
}

void UpdateGun(Player *player)
{
    Vector2 mouse_pos = GetMousePosition();
    Vector2 player_pos = player->position;
    Vector2 diff = Vector2Subtract(mouse_pos, player_pos);
    Vector2 gun_pos_relative = Vector2Scale(Vector2Normalize(diff), 25);
    player->gun_pos = Vector2Add(player_pos, gun_pos_relative);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_ENTER))
    {
        ShootGun(player);
    }

    if (IsKeyDown(KEY_B))
    {
        printf("\n\n\n-----PRINTING BULLETS-----\n");
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!_bullets[i].active) continue;
            printf("(%f, %f)\n", _bullets[i].hitbox.x, _bullets[i].hitbox.y);
        }
        printf("\n\n\n-----BULLETS PRINTED-----\n");
    }

    UpdateBullets();
}