#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

#include <raylib.h>

#include "ball.h"
#include "paddle.h"

bool DRAW_DEBUG = true;

int RIGHT_SCORE = 10;
int LEFT_SCORE = 10;
int TOP_SCORE = 10;
int BOTTOM_SCORE = 10;

const int INIT_SCREEN_W = 1280;
const int INIT_SCREEN_H = 720;

const int HALF_W = INIT_SCREEN_W / 2;
const int HALF_H = INIT_SCREEN_H / 2;

const int QUART_W = INIT_SCREEN_W / 4;
const int QUART_H = INIT_SCREEN_H / 4;

const int SXTNTH_W = INIT_SCREEN_W / 16;
const int SXTNTH_H = INIT_SCREEN_H / 16;

const int FONT_SIZE = 32;
const int TEXT_OFFSET = FONT_SIZE / 2;
const int TEXT_POS = TEXT_OFFSET * 3;

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
    DrawText(TextFormat("%i", LEFT_SCORE), PLAYGROUND_X + TEXT_OFFSET, TEXT_OFFSET, FONT_SIZE, RED);
    DrawText(TextFormat("%i", TOP_SCORE), PLAYGROUND_W - TEXT_POS, TEXT_OFFSET, FONT_SIZE, GREEN);
    DrawText(TextFormat("%i", RIGHT_SCORE), PLAYGROUND_W - TEXT_POS, INIT_SCREEN_H - TEXT_POS, FONT_SIZE, BLUE);
    DrawText(TextFormat("%i", BOTTOM_SCORE),   PLAYGROUND_X + TEXT_OFFSET, INIT_SCREEN_H - TEXT_POS, FONT_SIZE, YELLOW);
}

void CollisionBallPaddle(Ball * ball, Paddle * paddle) {
    if(CheckCollisionCircleRec(
        ball->position, 
        ball->radius, 
        paddle->bounds)) {


        Vector2 mtd = vm.GetMinimumTranslation(ball->position, ball->radius, paddle->bounds);
        ball->position = vm.Add(ball->position, mtd);

        // Left side
        if(ball->position.x < paddle->position.x) ball->direction.x = -ball->direction.x;
        // Top Side
        if(ball->position.y < paddle->position.y) ball->direction.y = -ball->direction.y;

        // Right side
        if(ball->position.x > paddle->position.x + paddle->w) ball->direction.x = -ball->direction.x;

        // Bottom side
        if(ball->position.y > paddle->position.y + paddle->h) ball->direction.y = -ball->direction.y;

        ball->velocity = vm.Scale(ball->direction, ball->speed);
    }
}

void CollisionBallBounce(Ball * ball, Rectangle rect) {
    if(CheckCollisionCircleRec(
        ball->position, 
        ball->radius, 
        rect)) {

        Vector2 mtd = vm.GetMinimumTranslation(ball->position, ball->radius, rect);
        Vector2 randDirection = { rand() % 2 == 0 ? 0.9f : 1.1f, rand() % 2 == 0 ? 0.9f : 1.1f };

        // Left side
        if(ball->position.x <= rect.x) ball->direction.x = -ball->direction.x * randDirection.x;
        // Top Side
        if(ball->position.y <= rect.y) ball->direction.y = -ball->direction.y * randDirection.y;

        // Right side
        if(ball->position.x >= rect.x + rect.width) ball->direction.x = -ball->direction.x * randDirection.x;
        // Bottom side
        if(ball->position.y >= rect.y + rect.height) ball->direction.y = -ball->direction.y * randDirection.y;

        ball->speed *= 1.1f;
        ball->velocity = vm.Scale(ball->direction, ball->speed);
        ball->position = vm.Add(ball->position, vm.Scale(mtd, 3.0f));
    }
}


void CollisionBallBorder(Ball * ball) {
    if(!CheckCollisionCircleRec(
    ball->position, 
    ball->radius, 
    Rectangle {PLAYGROUND_X, 0, INIT_SCREEN_H, INIT_SCREEN_H})) {     
        if (ball->position.y < 0) {
            Vector2 mtd = vm.GetMinimumTranslation(ball->position, ball->radius, {ball->position.x, 0, PLAYGROUND_W, 1});
            ball->position = vm.Add(ball->position, mtd);
            TOP_SCORE--;
        } else if (ball->position.y > INIT_SCREEN_H){
            Vector2 mtd = vm.GetMinimumTranslation(ball->position, ball->radius, {ball->position.x, INIT_SCREEN_H, PLAYGROUND_W, 1});
            ball->position = vm.Add(ball->position, mtd);
            BOTTOM_SCORE--;
        } else if (ball->position.x < PLAYGROUND_X) {
            Vector2 mtd = vm.GetMinimumTranslation(ball->position, ball->radius, {ball->position.x, 0, PLAYGROUND_W, 1});
            ball->position = vm.Add(ball->position, mtd);
            LEFT_SCORE--;
        } else if (ball->position.x > PLAYGROUND_W){
            Vector2 mtd = vm.GetMinimumTranslation(ball->position, ball->radius, {ball->position.x, INIT_SCREEN_H, PLAYGROUND_W, 1});
            ball->position = vm.Add(ball->position, mtd);
            RIGHT_SCORE--;
        }

        ball->Reset();
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

    int longSide = 64;
    int shortSide = 8;
    float square = 64.0f;

    Paddle leftPaddle{ Vector2 {(float) PLAYGROUND_X, (float)  HALF_H - longSide / 2}, shortSide, longSide, RED };
    Paddle topPaddle{ Vector2 {(float) HALF_W - longSide / 2, 0}, longSide, shortSide, GREEN};
    Paddle rightPaddle{ Vector2 { (float) PLAYGROUND_W - shortSide, (float) HALF_H - longSide / 2}, shortSide, longSide, BLUE };
    Paddle bottomPaddle{ Vector2 {(float) HALF_W - longSide / 2, (float) INIT_SCREEN_H - shortSide}, longSide, shortSide, YELLOW};

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

        // Spawner Rectangles
        DrawRectangleRec(topLeft, WHITE);
        DrawRectangleRec(topRight, WHITE);
        DrawRectangleRec(bottomLeft, WHITE);
        DrawRectangleRec(bottomRight, WHITE);

        // Collision detection
        if(ball.position.x < HALF_W) CollisionBallBounce(&ball, leftPaddle.bounds);
        else CollisionBallBounce(&ball, rightPaddle.bounds);
        if(ball.position.y < HALF_H) CollisionBallBounce(&ball, topPaddle.bounds);
        else CollisionBallBounce(&ball, bottomPaddle.bounds);

        if(ball.position.x < HALF_W) {
            if(ball.position.y < HALF_H) CollisionBallBounce(&ball, topLeft);
            else CollisionBallBounce(&ball, bottomLeft);
        }
        else {
            if(ball.position.y < HALF_H) CollisionBallBounce(&ball, topRight);
            else CollisionBallBounce(&ball, bottomRight);
        }

        CollisionBallBorder(&ball);

        // Updates
        ball.Update();
        leftPaddle.Update();
        rightPaddle.Update();
        topPaddle.Update();
        bottomPaddle.Update();

        // AI Movement
        leftPaddle.Auto(&ball, topLeft, bottomLeft);
        topPaddle.Auto(&ball, topLeft, topRight);
        bottomPaddle.Auto(&ball, bottomLeft, bottomRight);
        rightPaddle.Auto(&ball, topRight, bottomRight);
        // // Player Movement
        // rightPaddle.Input();

        // Debug Drawings
        if(DRAW_DEBUG){
            DrawText(TextFormat("(X: %i, Y: %i}", (int) ball.position.x, (int) ball.position.y), ball.position.x + 16, ball.position.y - 12, FONT_SIZE, WHITE);
            DrawLine(ball.position.x, ball.position.y, leftPaddle.center.x, leftPaddle.center.y, RED);
            DrawLine(ball.position.x, ball.position.y, topPaddle.center.x, topPaddle.center.y, GREEN);
            DrawLine(ball.position.x, ball.position.y, rightPaddle.center.x, rightPaddle.center.y, BLUE);
            DrawLine(ball.position.x, ball.position.y, bottomPaddle.center.x, bottomPaddle.center.y, YELLOW);
        }

        DrawScores();
        EndDrawing();
    }

    // Close the window and unload all resources
    CloseWindow();

    return 0;
}
