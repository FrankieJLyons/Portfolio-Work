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
        setVelocity(5.0f, Vector2 { (float) randomX, (float) randomY });
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
    int x, y, w, h;
    bool rotated = false;

    Paddle(int x, int y, int w, int h): x{x}, y{y}, w{w}, h{h} {}
    Paddle(int x, int y, int w, int h, bool rotated): x{x}, y{y}, w{w}, h{h}, rotated{rotated} {}
        
    void Print() {
		std::cout << "Paddle(X: " << x << ", Y: " << y << ", S: " << s << ")\n";
    }

    void Update() {
        Draw();
        CollisionBorder();
    }

    void Input() {
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && y > 0) y -= s * GetFrameTime();
        if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && y < GetScreenHeight() - h) y += s * GetFrameTime();
    }

    void Auto(Ball * ball) {
        if(rotated) {
            if (ball->position.x > x + w / 2) x += s * GetFrameTime();
            else if (ball->position.x <= x + w / 2) x -= s * GetFrameTime();
        } else {
            if (ball->position.y > y + h / 2) y += s * GetFrameTime();
            else if (ball->position.y <= y + h / 2) y -= s * GetFrameTime();
        }
    }

    private:
    int s = 420;

    void Draw() {
        DrawRectangle(x, y, w, h, WHITE);
    }

    void CollisionBorder() {
        if(rotated) {
            if(x + w >= GetScreenWidth()) {
                x = GetScreenWidth() - w;
            } else if(x <= 0) {
                x = 0;
            }
        } else {
            if(y + h >= GetScreenHeight()) {
                y = GetScreenHeight() - h;
            } else if(y <= 0) {
                y = 0;
            }
        }
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
        Rectangle {(float) paddle->x, (float) paddle->y, (float) paddle->w, (float) paddle->h})) {
            if(rotated) {
                if(ball->velocity.y < 0) ball->position.y = paddle->y + paddle->h + ball->radius + 1;
                else ball->position.y = paddle->y - ball->radius;
                ball->velocity.y *= -1;
            } else {
                if(ball->velocity.x < 0) ball->position.x = paddle->x + paddle->w + ball->radius + 1;
                else ball->position.x = paddle->x - ball->radius;
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
    Ball ball{ 8 };
    Paddle leftPaddle{ 32,  GetScreenHeight() / 2 - 64, 16, 128};
    Paddle rightPaddle{ GetScreenWidth() - 32 - 16,  GetScreenHeight() / 2 - 64, 16, 128};

    Paddle topPaddle{ GetScreenWidth() / 2 - 64, 32, 128, 16, true};
    Paddle bottomPaddle{ GetScreenWidth() / 2 - 64, GetScreenHeight() - 32 - 16, 128, 16, true};

    // Pick a random direction to multiply velocity by
    int randomX = rand() % 2 == 0 ? 1 : -1;
    int randomY = rand() % 2 == 0 ? 1 : -1;
    ball.setVelocity(5.0f, Vector2 { (float) randomX, (float) randomY });

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

        bottomPaddle.Print();
        ball.Print();

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
