#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include <raylib.h>

#include "ball.h"
#include "paddle.h"

int PLAYER_SCORE = 0;
int AI_SCORE = 0;

const int INIT_SCREEN_W = 1280;
const int INIT_SCREEN_H = 720;
const int HALF_W = INIT_SCREEN_W / 2;
const int QUART_W = INIT_SCREEN_W / 4;
const int SXTNTH_H = INIT_SCREEN_H / 16;
const int TEXT_OFFSET = 8;
const int FONT_SIZE = 32;

void UpdateRay() {
    ClearBackground(BLACK);
    DrawFPS(4, 4);
}

void UpdateBackground() {
    DrawLine(HALF_W, 0, HALF_W, GetScreenHeight(), WHITE);
    DrawText(TextFormat("%i", AI_SCORE), QUART_W - TEXT_OFFSET, SXTNTH_H, FONT_SIZE, WHITE);
    DrawText(TextFormat("%i", PLAYER_SCORE), (GetScreenWidth() - QUART_W) - TEXT_OFFSET, SXTNTH_H, FONT_SIZE, WHITE);
}

void CollisionBallPaddle(Ball * ball, Paddle * paddle, bool rotated) {
    if(CheckCollisionCircleRec(
        ball->position, 
        ball->radius, 
        Rectangle {(float) paddle->position.x, (float) paddle->position.y, (float) paddle->w, (float) paddle->h})) {
            if(rotated) {
                if(ball->velocity.y < 0) ball->position.y = paddle->position.y + paddle->h + ball->radius + 1;
                else ball->position.y = paddle->position.y - ball->radius;
                ball->velocity.y *= -1;
            } else {
                if(ball->velocity.x < 0) ball->position.x = paddle->position.x + paddle->w + ball->radius + 1;
                else ball->position.x = paddle->position.x - ball->radius;
                ball->velocity.x *= -1;
            }
    }
}

void CollisionBallBorder(Ball * ball) {
    if(ball->position.x > GetScreenWidth()) {
        ball->Reset();
        AI_SCORE++;
    } else if(ball->position.x < 0) {
        ball->Reset();
        PLAYER_SCORE++;
    }

    if(ball->position.y > GetScreenHeight()) {
        ball->position.y = GetScreenHeight();
        ball->velocity.y *= -1;
    } else if(ball->position.y < 0) {
        ball->position.y = 0;
        ball->velocity.y *= -1;
    }
}

int main()
{
    // Initialize raylib
    InitWindow(INIT_SCREEN_W, INIT_SCREEN_H, "Pong");
    SetWindowState(FLAG_VSYNC_HINT);

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Initialize entities
    Ball ball;
    Paddle leftPaddle{ Vector2 { 32,  (float) GetScreenHeight() / 2 - 64} };
    Paddle rightPaddle{ Vector2 { (float) GetScreenWidth() - 32 - 16,  (float) GetScreenHeight() / 2 - 64} };

    Paddle topPaddle{ Vector2 {(float) GetScreenWidth() / 2 - 64, 32}, 128, 16, true};
    Paddle bottomPaddle{ Vector2 {(float) GetScreenWidth() / 2 - 64, (float) GetScreenHeight() - 32 - 16}, 128, 16, true};

    // Pick a random initial direction to multiply velocity by
    int randomX = rand() % 2 == 0 ? 1 : -1;
    int randomY = rand() % 2 == 0 ? 1 : -1;
    ball.setVelocity(4.0f, Vector2 { (float) randomX, (float) randomY });

    // Main game loop
    while (!WindowShouldClose())
    {
        // Draw background
        BeginDrawing();
        UpdateRay();
        UpdateBackground();

        // Collision detection
        if(ball.position.x < GetScreenWidth() / 2) CollisionBallPaddle(&ball, &leftPaddle, false);
        else CollisionBallPaddle(&ball, &rightPaddle, false);
        if(ball.position.y < GetScreenHeight() / 2) CollisionBallPaddle(&ball, &topPaddle, true);
        else CollisionBallPaddle(&ball, &bottomPaddle, true);

        CollisionBallBorder(&ball);

        // Updates
        leftPaddle.Update();
        rightPaddle.Update();
        topPaddle.Update();
        bottomPaddle.Update();
        ball.Update();

        // AI Movement
        leftPaddle.Auto(&ball);
        topPaddle.Auto(&ball);
        bottomPaddle.Auto(&ball);

        // Player
        rightPaddle.Input();

        EndDrawing();
    }

    // Close the window and unload all resources
    CloseWindow();

    return 0;
}
