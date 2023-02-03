#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

#include <raylib.h>

#include "ball.h"
#include "paddle.h"

bool DRAW_DEBUG = true;

int RIGHT_SCORE = 0;
int LEFT_SCORE = 0;
int TOP_SCORE = 0;
int BOTTOM_SCORE = 0;

const int INIT_SCREEN_W = 1280;
const int INIT_SCREEN_H = 720;

const int HALF_W = INIT_SCREEN_W / 2;
const int HALF_H = INIT_SCREEN_H / 2;

const int QUART_W = INIT_SCREEN_W / 4;
const int QUART_H = INIT_SCREEN_H / 4;

const int SXTNTH_W = INIT_SCREEN_W / 16;
const int SXTNTH_H = INIT_SCREEN_H / 16;

const int TEXT_OFFSET = 8;
const int FONT_SIZE = 32;

const int PLAYGROUND_X = HALF_W - HALF_H;
const int PLAYGROUND_W = HALF_W + HALF_H; 

VectorMath & vm = VectorMath::getInstance();

void UpdateRay() {
    ClearBackground(BLACK);
    if(DRAW_DEBUG) DrawFPS(4, 4);
    if(IsKeyPressed(KEY_F)) DRAW_DEBUG = !DRAW_DEBUG;
}

void DrawPlayground() {
    DrawRectangleLines(PLAYGROUND_X, 0, INIT_SCREEN_H, INIT_SCREEN_H, WHITE);
    DrawLine(HALF_W, 0, HALF_W, GetScreenHeight(), WHITE);
    DrawLine(PLAYGROUND_X, HALF_H, PLAYGROUND_W, HALF_H, WHITE);
}

void DrawScores() {
    DrawText(TextFormat("%i", LEFT_SCORE), QUART_W - TEXT_OFFSET, SXTNTH_H, FONT_SIZE, RED);
    DrawText(TextFormat("%i", TOP_SCORE), (INIT_SCREEN_W - QUART_W) - TEXT_OFFSET, SXTNTH_H, FONT_SIZE, GREEN);
    DrawText(TextFormat("%i", RIGHT_SCORE), (INIT_SCREEN_W - QUART_W) - TEXT_OFFSET, INIT_SCREEN_H - SXTNTH_H, FONT_SIZE, BLUE);
    DrawText(TextFormat("%i", BOTTOM_SCORE),  QUART_W - TEXT_OFFSET, INIT_SCREEN_H - SXTNTH_H, FONT_SIZE, YELLOW);
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

Vector2 GetMinimumTranslation(Ball * ball, Vector2 line) {
    Vector2 mtd = {0, 0};

    // Get the closest point on the rectangle to the center of the circle
    Vector2 closest = vm.Clamp(ball->position, line, vm.Add(line, { (float) GetScreenWidth(), 1}));
    
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


        Vector2 mtd = GetMinimumTranslation(ball, paddle);
        ball->position = vm.Add(ball->position, mtd);

        // Left side
        if(ball->position.x < paddle->position.x) ball->direction.x = -ball->direction.x;
        // Top Side
        if(ball->position.y < paddle->position.y) ball->direction.y = -ball->direction.y;

        // Right side
        if(ball->position.x > paddle->position.x + paddle->w) ball->direction.x = -ball->direction.x;

        // Bottom side
        if(ball->position.y > paddle->position.y + paddle->h) ball->direction.y = -ball->direction.y;
    }
}

void CollisionBallBorder(Ball * ball) {
    if(ball->position.x > GetScreenWidth()) {
        ball->Reset();
        LEFT_SCORE++;
    } else if(ball->position.x < 0) {
        ball->Reset();
        RIGHT_SCORE++;
    }

    if (ball->position.y - ball->radius <= 0) {
        Vector2 mtd = GetMinimumTranslation(ball, {ball->position.x, 0});
        ball->position = vm.Add(ball->position, mtd);
        ball->direction.y = -ball->direction.y;
    } else if (ball->position.y + ball->radius > GetScreenHeight()){
        Vector2 mtd = GetMinimumTranslation(ball, {ball->position.x, (float) GetScreenHeight()});
        ball->position = vm.Add(ball->position, mtd);
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

    int longSide = 64;
    int shortSide = 8;
    float square = 32.0f;

    Paddle leftPaddle{ Vector2 {(float) PLAYGROUND_X, (float)  HALF_H - longSide / 2}, shortSide, longSide };
    Paddle rightPaddle{ Vector2 { (float) PLAYGROUND_W - shortSide, (float) HALF_H - longSide / 2}, shortSide, longSide };
    Paddle topPaddle{ Vector2 {(float) HALF_W - longSide / 2, 0}, longSide, shortSide};
    Paddle bottomPaddle{ Vector2 {(float) HALF_W - longSide / 2, (float) INIT_SCREEN_H - shortSide}, longSide, shortSide};

    Rectangle topLeft{ (float) PLAYGROUND_X, 0, square, square };
    Rectangle topRight{  (float) PLAYGROUND_W - square, 0, square, square };
    Rectangle bottomLeft{ (float) PLAYGROUND_X, INIT_SCREEN_H - square, square, square};
    Rectangle bottomRight{ (float) PLAYGROUND_W - square, INIT_SCREEN_H - square, square, square};

    // Main game loop
    while (!WindowShouldClose())
    {
        // Draw background
        BeginDrawing();
        UpdateRay();
        DrawPlayground();
        DrawScores();

        // Collision detection
        if(ball.position.x < GetScreenWidth() / 2) CollisionBallPaddle(&ball, &leftPaddle);
        else CollisionBallPaddle(&ball, &rightPaddle);
        if(ball.position.y < GetScreenHeight() / 2) CollisionBallPaddle(&ball, &topPaddle);
        else CollisionBallPaddle(&ball, &bottomPaddle);

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

        // Player Movement
        rightPaddle.Input();

        // Debug Drawings
        if(DRAW_DEBUG){
            DrawText(TextFormat("(X: %i, Y: %i}", (int) ball.position.x, (int) ball.position.y), ball.position.x + 16, ball.position.y - 12, FONT_SIZE, WHITE);
            DrawLine(ball.position.x, ball.position.y, leftPaddle.center.x, leftPaddle.center.y, RED);
            DrawLine(ball.position.x, ball.position.y, topPaddle.center.x, topPaddle.center.y, GREEN);
            DrawLine(ball.position.x, ball.position.y, rightPaddle.center.x, rightPaddle.center.y, BLUE);
            DrawLine(ball.position.x, ball.position.y, bottomPaddle.center.x, bottomPaddle.center.y, YELLOW);
        }

        EndDrawing();
    }

    // Close the window and unload all resources
    CloseWindow();

    return 0;
}
