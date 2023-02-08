#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <forward_list>
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
    DrawText(TextFormat("%i", BOTTOM_SCORE), PLAYGROUND_X + TEXT_OFFSET, INIT_SCREEN_H - TEXT_POS, FONT_SIZE, GOLD);
}

// Ball / Rectangle bounce 
void bounceBallOffRectangle (Ball * ball, Rectangle rect) {
    // Going Up and Left
    if(ball->velocity.y < 0 && ball->velocity.x < 0) {
        // Bottom Right
        if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Back the way it came
            ball->direction.x *= -1;
            ball->direction.y *= -1;
        }
        // Top Right
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        }
        // Bottom Left
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        } 
        // Right 
        else if (vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Bottom 
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
    }

    // Going Up and Right
    else if(ball->velocity.y < 0 && ball->velocity.x > 0) {
        // Bottom Left
        if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Back the way it came
            ball->direction.x *= -1;
            ball->direction.y *= -1;
        } 
        // Top Left
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        }
        // Bottom Right
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
        // Left 
        else if (vm.isWithinRange(ball->position.x, rect.x , ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Bottom 
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
    }

    // Going Down and Right 
    else if(ball->velocity.y > 0 && ball->velocity.x > 0) {
        // Top Left
        if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Back the way it came
            ball->direction.x *= -1;
            ball->direction.y *= -1;
        }
        // Top Right
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
        // Bottom Left
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Left 
        else if (vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Top 
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
    }

            // Going Down and Left
    else if(ball->velocity.y > 0 && ball->velocity.x < 0) {
        // Top Right
        if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Back the way it came
            ball->direction.x *= -1;
            ball->direction.y *= -1;
        }
        // Top Left
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
        // Bottom Right
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        }
        // Right 
        else if (vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Top 
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
    }
}

void resolveBallRectCollision (Ball * ball, Vector2 mtd) {
        ball->direction = vm.Add(ball->direction, vm.Scale(vm.GetRandomDirection(), 0.05f));
        if(ball->speed <= ball->maxSpeed) ball->speed *= 1.05f;
        ball->velocity = vm.Scale(ball->direction, ball->speed);
        ball->position = vm.Add(ball->position, vm.Scale(mtd, 1.025f));
}

void CollisionBallPaddle(Ball * ball, Paddle paddle) {
    static double lastCollisionTime = 0.0;
    double currentTime = GetTime();
    double elapsedTime = currentTime - lastCollisionTime;
    Rectangle rect = paddle.bounds;
    if(CheckCollisionCircleRec(
        ball->position, 
        ball->radius, 
        rect) && elapsedTime > 0.1) {
        lastCollisionTime = currentTime;
        Vector2 next = {paddle.position.x + paddle.velocity.x * GetFrameTime(), paddle.position.y + paddle.velocity.y * GetFrameTime()};
        Vector2 mtd = vm.GetMinimumMovingTranslation(ball->position, ball->radius, rect, next);
        bounceBallOffRectangle(ball, rect);
        resolveBallRectCollision(ball, mtd);
    }
}

void CollisionBallBounce(Ball * ball, Rectangle rect) {
    if(CheckCollisionCircleRec(
        ball->position, 
        ball->radius, 
        rect)) {   
        Vector2 mtd = vm.GetMinimumTranslation(ball->position, ball->radius, rect);
        bounceBallOffRectangle(ball, rect);
        resolveBallRectCollision(ball, mtd);
    }
}

void CollisionBallBorder(forward_list<Ball>* balls, Ball * ball) {
    int buffer = ball->radius * 4.0f;
    if(!CheckCollisionCircleRec(
    ball->position, 
    buffer, 
    Rectangle {PLAYGROUND_X, 0, INIT_SCREEN_H, INIT_SCREEN_H})) {     
        if (ball->position.y < 0) {
            TOP_SCORE--;
        } else if (ball->position.y > INIT_SCREEN_H){
            BOTTOM_SCORE--;
        } else if (ball->position.x < PLAYGROUND_X) {
            LEFT_SCORE--;
        } else if (ball->position.x > PLAYGROUND_W){
            RIGHT_SCORE--;
        }

        // Delete ball
        auto it = balls->before_begin();
        auto end = balls->end();
        for (auto nextIt = balls->begin(); nextIt != end; it = nextIt, ++nextIt) {
            if (&(*nextIt) == ball) {
                balls->erase_after(it);
                break;
            }
        }
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
    forward_list<Ball> balls = { Ball() };

    int longSide = 96;
    int shortSide = 12;
    float square = 64.0f;

    Paddle topPaddle{ Vector2 {(float) HALF_W - longSide / 2, 0}, longSide, shortSide, GREEN};
    Paddle leftPaddle{ Vector2 {(float) PLAYGROUND_X, (float) HALF_H - longSide / 2}, shortSide, longSide, RED };
    Paddle rightPaddle{ Vector2 { (float) PLAYGROUND_W - shortSide, (float) HALF_H - longSide / 2}, shortSide, longSide, BLUE };
    Paddle bottomPaddle{ Vector2 {(float) HALF_W - longSide / 2, (float) INIT_SCREEN_H - shortSide}, longSide, shortSide, GOLD};

    Rectangle topLeft{ (float) PLAYGROUND_X, 0, square, square };
    Rectangle topRight{ (float) PLAYGROUND_W - square, 0, square, square };
    Rectangle bottomLeft{ (float) PLAYGROUND_X, INIT_SCREEN_H - square, square, square};
    Rectangle bottomRight{ (float) PLAYGROUND_W - square, INIT_SCREEN_H - square, square, square};

    // Set up timer to add new balls
    float addBallInterval = 3.0f; // time in seconds
    float timeSinceLastBall = 0.0f;

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

        leftPaddle.Update();
        rightPaddle.Update();
        topPaddle.Update();
        bottomPaddle.Update();

        // Add a new ball every `addBallInterval` seconds
        timeSinceLastBall += GetFrameTime();
        if (timeSinceLastBall >= addBallInterval) {
            timeSinceLastBall = 0.0f;
            balls.push_front(Ball());
        }

        // BALLS
        for (Ball & ball : balls) {
            ball.Update();
            // Collision detection
            if(ball.position.x < HALF_W) CollisionBallPaddle(&ball, leftPaddle);
            else CollisionBallPaddle(&ball, rightPaddle);
            if(ball.position.y < HALF_H) CollisionBallPaddle(&ball, topPaddle);
            else CollisionBallPaddle(&ball, bottomPaddle);

            if(ball.position.x < HALF_W) {
                if(ball.position.y < HALF_H) CollisionBallBounce(&ball, topLeft);
                else CollisionBallBounce(&ball, bottomLeft);
            }
            else {
                if(ball.position.y < HALF_H) CollisionBallBounce(&ball, topRight);
                else CollisionBallBounce(&ball, bottomRight);
            }
            CollisionBallBorder(&balls, &ball);
        }

        // AI Movement
        leftPaddle.Auto(balls, topLeft, bottomLeft);
        topPaddle.Auto(balls, topLeft, topRight);
        bottomPaddle.Auto(balls, bottomLeft, bottomRight);
        rightPaddle.Auto(balls, topRight, bottomRight);
        // // Player Movement
        // rightPaddle.Input();

        // Debug Drawings
        if(DRAW_DEBUG){
            for (Ball & ball : balls) {
                DrawText(TextFormat("X: %i", (int) ball.position.x), 8, 32, 16, WHITE);
                DrawText(TextFormat("Y: %i", (int) ball.position.y), 8, 48, 16, WHITE);
                DrawText(TextFormat("S: %i", (int) ball.speed), 8, 64, 16, WHITE);
                DrawLine(ball.position.x, ball.position.y, leftPaddle.center.x, leftPaddle.center.y, RED);
                DrawLine(ball.position.x, ball.position.y, topPaddle.center.x, topPaddle.center.y, GREEN);
                DrawLine(ball.position.x, ball.position.y, rightPaddle.center.x, rightPaddle.center.y, BLUE);
                DrawLine(ball.position.x, ball.position.y, bottomPaddle.center.x, bottomPaddle.center.y, GOLD);
            }
        }

        DrawScores();
        EndDrawing();
    }

    // Close the window and unload all resources
    CloseWindow();

    return 0;
}
