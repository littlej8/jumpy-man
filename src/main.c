#include "raylib.h"
#include "player.h"
#include "obstacles.h"
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void Init(void);
void Update(void);
void Draw(void);

Player _player;

int _paused = 0;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "jumpy man");
    SetTargetFPS(60);

    Init();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P))
        {
            _paused = !_paused;
        }

        Update();
        BeginDrawing();
            ClearBackground(RAYWHITE);
            Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void Init(void)
{
    _player = (Player){
        .position = (Vector2){SCREEN_WIDTH/2 - 10/2, SCREEN_HEIGHT/3},
        .velocity = (Vector2){0, 0},
        .size = (Vector2){10, 20},
        .max_speed = 10,
        .acceleration = 1,
        .friction = 0.75,
        .jump_power = 15,
        .gravity = 1,
        .gun_pos = (Vector2){0, 0}
    };

    AddObstacle((Obstacle){(Rectangle){0, SCREEN_HEIGHT-10, SCREEN_WIDTH, 10}, BLACK}); // floor
    AddObstacle((Obstacle){(Rectangle){0, 0, 10, SCREEN_HEIGHT}, BLACK}); // left wall
    AddObstacle((Obstacle){(Rectangle){0, 0, SCREEN_WIDTH, 10}, BLACK}); // ceiling
    AddObstacle((Obstacle){(Rectangle){SCREEN_WIDTH-10, 0, 10, SCREEN_HEIGHT}, BLACK}); // right wall

    AddObstacle((Obstacle){(Rectangle){200, 400, 200, 50}, LIGHTGRAY});
}

void Update(void)
{
    if (_paused) return;
    UpdatePlayer(&_player);
}

void Draw(void)
{
    if (_paused) return;
    DrawObstacles();

    DrawPlayer(_player);

    const char *text = TextFormat("%d", CheckObstacleCollision((Rectangle){_player.position.x-_player.size.x/2, _player.position.y-_player.size.y/2, _player.size.x, _player.size.y}));
    DrawText(text, SCREEN_WIDTH/2 - MeasureText(text, 20)/2, SCREEN_HEIGHT/2, 20, DARKGRAY);
}