#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

#include <raylib.h>

#include "ball.h"
#include "paddle.h"

bool DRAW_FPS = true;

int PLAYER_SCORE = 0;
int AI_SCORE = 0;

const int INIT_SCREEN_W = 1280;
const int INIT_SCREEN_H = 720;
const int HALF_W = INIT_SCREEN_W / 2;
const int QUART_W = INIT_SCREEN_W / 4;
const int SXTNTH_H = INIT_SCREEN_H / 16;
const int TEXT_OFFSET = 8;
const int FONT_SIZE = 32;

VectorMath & vm = VectorMath::getInstance();

void UpdateRay() {
    ClearBackground(BLACK);
    if(DRAW_FPS) DrawFPS(4, 4);
    if(IsKeyPressed(KEY_F)) DRAW_FPS = !DRAW_FPS;
}

void UpdateBackground() {
    DrawLine(HALF_W, 0, HALF_W, GetScreenHeight(), WHITE);
    DrawText(TextFormat("%i", AI_SCORE), QUART_W - TEXT_OFFSET, SXTNTH_H, FONT_SIZE, WHITE);
    DrawText(TextFormat("%i", PLAYER_SCORE), (GetScreenWidth() - QUART_W) - TEXT_OFFSET, SXTNTH_H, FONT_SIZE, WHITE);
}

// Ball and Paddle Specific
float GetDistance(Ball * ball, Paddle * paddle) {
  Vector2 closest = vm.Clamp(ball->position, paddle->position, vm.Add(paddle->position, {(float) paddle->w, (float) paddle->h}));
  Vector2 distance = vm.Subtract(ball->position, closest);
  return vm.Length(distance) - ball->radius;
}

Vector2 GetMinimumTranslation(Ball * ball, Paddle * paddle) {
    Vector2 mtd = {0, 0};

    // Get the closest point on the rectangle to the center of the circle
    Vector2 closest = vm.Clamp(ball->position, paddle->position, vm.Add(paddle->position, {(float) paddle->w, (float) paddle->h}));
    
    // Get the distance between the closest point and the center of the circle
    Vector2 distance = vm.Subtract(ball->position, closest);
    
    // If the distance is less than the radius of the circle, there is overlap
    float d = vm.Length(distance);
    if (d < ball->radius) {
        // Calculate the MTD by normalizing the distance and scaling it by the distance minus the radius of the circle
        mtd = vm.Scale(vm.Normalize(distance), ball->radius - d);
    }

    return mtd;
}

void CollisionBallPaddle(Ball * ball, Paddle * paddle) {
    if(CheckCollisionCircleRec(
        ball->position, 
        ball->radius, 
        paddle->bounds)) {

        ball->Print();

        Vector2 mtd = GetMinimumTranslation(ball, paddle);
        ball->position = vm.Add(ball->position, mtd);
        
        Vector2 closest = {
            fmaxf(paddle->position.x, fminf(ball->position.x, paddle->position.x + paddle->w)),
            fmaxf(paddle->position.y, fminf(ball->position.y, paddle->position.y + paddle->h))
        };
        
        float incomingAngle = atan2(ball->position.y - paddle->center.y, ball->position.x - paddle->center.x);

        Vector2 surfaceNormal = vm.Normalize(vm.Subtract(closest, ball->position));
        float surfaceNormalAngle = atan2(surfaceNormal.y, surfaceNormal.x);
        float reflection = (2 * surfaceNormalAngle) - incomingAngle;

        ball->direction = vm.Normalize({cos(reflection), sin(reflection)});
        ball->velocity = vm.Scale(ball->direction, ball->speed);

        ball->Print();
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

    if (ball->position.y - ball->radius < 0 || ball->position.y + ball->radius >= GetScreenHeight()) {
        ball->direction.y = -ball->direction.y;
    }

    ball->velocity = vm.Scale(ball->direction, ball->speed);
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

    // Paddle topPaddle{ Vector2 {(float) GetScreenWidth() / 2 - 64, 32}, 128, 16};
    // Paddle bottomPaddle{ Vector2 {(float) GetScreenWidth() / 2 - 64, (float) GetScreenHeight() - 32 - 16}, 128, 16};

    // Main game loop
    while (!WindowShouldClose())
    {
        // Draw background
        BeginDrawing();
        UpdateRay();
        UpdateBackground();

        // Collision detection
        CollisionBallPaddle(&ball, &leftPaddle);
        CollisionBallPaddle(&ball, &rightPaddle);
        // CollisionBallPaddle(&ball, &topPaddle);
        // CollisionBallPaddle(&ball, &bottomPaddle);

        CollisionBallBorder(&ball);

        // Updates
        leftPaddle.Update();
        rightPaddle.Update();
        // topPaddle.Update();
        // bottomPaddle.Update();
        ball.Update();

        // AI Movement
        leftPaddle.Auto(&ball);
        // topPaddle.Auto(&ball);
        // bottomPaddle.Auto(&ball);

        // Player
        rightPaddle.Input();



        DrawLine(ball.position.x, ball.position.y, leftPaddle.center.x, leftPaddle.center.y, RED);

        EndDrawing();
    }

    // Close the window and unload all resources
    CloseWindow();

    return 0;
}
