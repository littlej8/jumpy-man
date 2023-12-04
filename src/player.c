#include "player.h"

#include "raylib.h"
#include "raymath.h"
#include "obstacles.h"
#include "guns.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PLAYER_TRAIL_MAX_LEN 15
Rectangle _player_trail[PLAYER_TRAIL_MAX_LEN];
size_t _player_trail_len = 0;

void DrawPlayerTrail(Rectangle newRect)
{
    if (_player_trail_len == 0)
    {
        for (int i = 0; i < PLAYER_TRAIL_MAX_LEN; i++)
        {
            _player_trail[i] = (Rectangle){-1, -1, -1, -1};
        }
    }

    if (_player_trail_len < PLAYER_TRAIL_MAX_LEN)
    {
        _player_trail[_player_trail_len++] = newRect;
    }
    else
    {
        for (int i = 0; i < PLAYER_TRAIL_MAX_LEN - 1; i++)
        {
            _player_trail[i] = _player_trail[i + 1];
        }
        _player_trail[PLAYER_TRAIL_MAX_LEN - 1] = newRect;
    }

    for (int i = 0; i < PLAYER_TRAIL_MAX_LEN; i++)
    {
        if (_player_trail[i].x == -1) break;
        Rectangle rect = _player_trail[i];
        float scale = (i + 1) / (float)_player_trail_len;

        float transparency = 255 * scale;
        float width = rect.width * scale;
        float height = rect.height * scale;
        DrawRectangle(rect.x + (rect.width - width)/2, rect.y + (rect.height - height), width, height, (Color){230, 41, 55, transparency});
    }
}

void DrawPlayer(Player player)
{
    Rectangle pos = {
        .x = player.position.x - player.size.x / 2,
        .y = player.position.y - player.size.y / 2,
        .width = player.size.x,
        .height = player.size.y
    };
    DrawRectangle(pos.x, pos.y, pos.width, pos.height, RED);
    DrawPlayerTrail(pos);
    DrawGun(player);
}

int PlayerCollision(Player *player)
{
    Rectangle rect = {player->position.x-player->size.x/2, player->position.y-player->size.y/2, player->size.x, player->size.y};
    return CheckObstacleCollision(rect);
}

int PlayerOnFloor(Player *player)
{
    return player->position.y > GetScreenHeight() - 10 - player->size.y/2;
}

int PlayerHitCeiling(Player *player)
{
    return player->position.y < 10 + player->size.y/2;
}

void UpdatePlayer(Player *player)
{
    player->position.x += player->velocity.x;
    int noclip = PlayerCollision(player);

    if (noclip)
    {
        player->position.x -= player->velocity.x;
        player->velocity.x = 0;
    }

    player->position.y += player->velocity.y;

    int on_floor = PlayerOnFloor(player);
    int on_ceiling = PlayerHitCeiling(player);
    noclip = PlayerCollision(player);

    if (on_ceiling || noclip)
    {
        player->position.y -= player->velocity.y;
        player->velocity.y = 0;
    }

    if (on_floor || noclip)
    {
        player->position.y -= player->velocity.y;
        player->velocity.y = 0;
    }
    
    if (!on_floor && !noclip)
    {
        player->velocity.y += player->gravity;
    }
    
    int accelerating = 0;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        accelerating = 1;
        player->velocity.x -= player->acceleration;
        if (fabsf(player->velocity.x) > player->max_speed)
        {
            player->velocity.x = -player->max_speed;
        }
    }
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        accelerating = 1;
        player->velocity.x += player->acceleration;
        if (fabsf(player->velocity.x) > player->max_speed)
        {
            player->velocity.x = player->max_speed;
        }
    }

    if (!accelerating)
    {
        player->velocity.x *= player->friction;
    }

    if (fabsf(player->velocity.x) <= 0.01)
    {
        player->velocity.x = 0;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        player->velocity.y = -player->jump_power;
    }

    UpdateGun(player);
}