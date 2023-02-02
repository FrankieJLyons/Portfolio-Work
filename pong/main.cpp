#include <raylib.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int PLAYER_SCORE = 0;
int AI_SCORE = 0;

class Ball {
    public:
    int radius;
    Vector2 position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    Vector2 velocity = { 0.0f, 0.0f };

    Ball(): radius{8} {}
    Ball(int r): radius{r} {}

    void Print() {
		std::cout << "Ball Pos: X: " << position.x << ", Y: " << position.y << "\n";
		std::cout << "Ball Vel: X: " << velocity.x << ", Y: " << velocity.y << "\n";
    }

    void setVelocity(float speed, Vector2 direction) { 
        velocity = Vector2Multiply(Vector2 {speed, speed}, direction);
    }

    void Update() {
        Draw();
        CollisionBorder();

        position = Vector2Add(position, velocity);
    }

    private:
    void Draw() {
        DrawCircle(position.x, position.y, radius, WHITE);
    }

    void Reset() {
        position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        velocity = { 0.0f, 0.0f };

        int randomX = rand() % 2 == 0 ? 1 : -1;
        int randomY = rand() % 2 == 0 ? 1 : -1;
        setVelocity(4.0f, Vector2 { (float) randomX, (float) randomY });
    }

    void CollisionBorder() {
        if(position.x > GetScreenWidth()) {
            Reset();
            AI_SCORE++;
        } else if(position.x < 0) {
            Reset();
            PLAYER_SCORE++;
        }

        if(position.y > GetScreenHeight()) {
            position.y = GetScreenHeight();
            velocity.y *= -1;
        } else if(position.y < 0) {
            position.y = 0;
            velocity.y *= -1;
        }
    }

    Vector2 Vector2Add(Vector2 one, Vector2 two) {
        return {one.x + two.x, one.y + two.y};
    }

    Vector2 Vector2Multiply(Vector2 one, Vector2 two) {
        return {one.x * two.x, one.y * two.y};
    }
};

class Paddle {
    public:
    Vector2 position;
    Vector2 velocity = { 300.0f, 300.0f };
    int w = 16; 
    int h = 128;
    bool rotated = false;
    int autoOffset = 4;
    int centerX = position.x + (w / 2);
    int centerY = position.y + (h / 2);

    Paddle(Vector2 position): position{position} {}
    Paddle(Vector2 position, int w, int h): position{position}, w{w}, h{h} {}
    Paddle(Vector2 position, int w, int h, bool rotated): position{position}, w{w}, h{h}, rotated{rotated} {}
        
    void Print() {
		std::cout << "Paddle Pos: X: " << position.x << ", Y: " << position.y << "\n";
		std::cout << "Paddle Vel: X: " << velocity.x << ", Y: " << velocity.y << "\n";
    }

    void Update() {
        Draw();
        CollisionBorder();
    }

    void Input() {
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && position.y > 0) position.y -= velocity.y * GetFrameTime();
        if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && position.y < GetScreenHeight() - h) position.y += velocity.y * GetFrameTime();
    }

    void Auto(Ball * ball) {
        centerX = position.x + (w / 2);
        centerY = position.y + (h / 2);

        if(rotated) {
            if (ball->position.x > centerX + autoOffset) position.x += velocity.x * GetFrameTime();
            else if (ball->position.x <= centerX - autoOffset) position.x -= velocity.x * GetFrameTime();
        } else {
            if (ball->position.y > centerY + autoOffset) position.y += velocity.y * GetFrameTime();
            else if (ball->position.y <= centerY - autoOffset) position.y -= velocity.y * GetFrameTime();
        }
    }

    private:
    void Draw() {
        DrawRectangle(position.x, position.y, w, h, WHITE);
    }

    void CollisionBorder() {
        if(rotated) {
            if(position.x + w >= GetScreenWidth()) {
                position.x = GetScreenWidth() - w;
            } else if(position.x <= 0) {
                position.x = 0;
            }
        } else {
            if(position.y + h >= GetScreenHeight()) {
                position.y = GetScreenHeight() - h;
            } else if(position.y <= 0) {
                position.y = 0;
            }
        }
    }

    Vector2 Vector2Add(Vector2 one, Vector2 two) {
        return {one.x + two.x, one.y + two.y};
    }

    Vector2 Vector2Multiply(Vector2 one, Vector2 two) {
        return {one.x * two.x, one.y * two.y};
    }
};

void UpdateRay() {
    ClearBackground(BLACK);
    DrawFPS(4, 4);
}

void UpdateBackground() {

    int halfWidth = GetScreenWidth() / 2;
    int quarterWidth = GetScreenWidth() / 4;
    int sixteenthHeight = GetScreenHeight() / 16;
    int offset = 8;
    int fontSize = 32;

    DrawLine(halfWidth, 0, halfWidth, GetScreenHeight(), WHITE);
    DrawText(TextFormat("%i", AI_SCORE), quarterWidth - offset, sixteenthHeight, fontSize, WHITE);
    DrawText(TextFormat("%i", PLAYER_SCORE), (GetScreenWidth() - quarterWidth) - offset, sixteenthHeight, fontSize, WHITE);
}

void CollisionCheck(Ball * ball, Paddle * paddle, bool rotated) {
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

int main()
{
    int m_screen_w = 1280;
    int m_screen_h = 720;

    // Initialize raylib
    InitWindow(m_screen_w, m_screen_h, "Pong");
    SetWindowState(FLAG_VSYNC_HINT);

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Initialize entities
    Ball ball;
    Paddle leftPaddle{ Vector2 { 32,  (float) GetScreenHeight() / 2 - 64} };
    Paddle rightPaddle{ Vector2 { (float) GetScreenWidth() - 32 - 16,  (float) GetScreenHeight() / 2 - 64} };

    Paddle topPaddle{ Vector2 {(float) GetScreenWidth() / 2 - 64, 32}, 128, 16, true};
    Paddle bottomPaddle{ Vector2 {(float) GetScreenWidth() / 2 - 64, (float) GetScreenHeight() - 32 - 16}, 128, 16, true};

    // Pick a random direction to multiply velocity by
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
        if(ball.position.x < GetScreenWidth() / 2) CollisionCheck(&ball, &leftPaddle, false);
        else CollisionCheck(&ball, &rightPaddle, false);
        if(ball.position.y < GetScreenHeight() / 2) CollisionCheck(&ball, &topPaddle, true);
        else CollisionCheck(&ball, &bottomPaddle, true);

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
