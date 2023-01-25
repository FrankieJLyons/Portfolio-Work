#include <raylib.h>
#include <iostream>
using namespace std;

int PLAYER_SCORE = 0;
int AI_SCORE = 0;

class Ball {
    public:
    int x, y, s_x, s_y, r;

    Ball(int x, int y, int s_x, int s_y, int r): x{x}, y{y}, s_x{s_x}, s_y{s_y}, r{r} {}

    void Update() {
        Draw();
        CollisionBorder();
        // Print();

        x += s_x * GetFrameTime();
        y += s_y * GetFrameTime();
    }

    private:
    void Draw() {
        DrawCircle(x, y, r, WHITE);
    }

    void Reset() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
        s_x *= -1;
    }

    void CollisionBorder() {
        if(x > GetScreenWidth()) {
            Reset();
            AI_SCORE++;
        } else if(x < 0) {
            Reset();
            PLAYER_SCORE++;
        }

        if(y > GetScreenHeight()) {
            y = GetScreenHeight();
            s_y *= -1;
        } else if(y < 0) {
            y = 0;
            s_y *= -1;
        }
    }

    void Print() {
		std::cout << "Ball(X: " << s_x << ", Y: " << y << ")\n";
    }
};

class Paddle {
    public:
    int x, y;
    int w = 16;
    int h = 128;

    Paddle(int x, int y): x{x}, y{y} {}

    void Update() {
        Draw();
        CollisionBorder();
        // Print();
    }

    void Input() {
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && y > 0) y -= s * GetFrameTime();
        if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && y < GetScreenHeight() - h) y += s * GetFrameTime();
    }

    void Auto(int ball_y) {
        if (ball_y > y + h / 2) y += s * GetFrameTime();
        else if (ball_y <= y + h / 2) y -= s * GetFrameTime();
    }

    private:
    int s = 420;

    void Draw() {
        DrawRectangle(x, y, w, h, WHITE);
    }

    void CollisionBorder() {
        if(y + h >= GetScreenHeight()) {
            y = GetScreenHeight() - h;
        } else if(y <= 0) {
            y = 0;
        }
    }

    void Print() {
		std::cout << "Paddle(Y: " << y << ", S: " << s << ")\n";
    }
};

void UpdateRay() {
    ClearBackground(BLACK);
    // DrawFPS(4, 4);
}

void UpdateBackground() {
    DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), WHITE);
    DrawText(TextFormat("%i", AI_SCORE), (GetScreenWidth() / 4) - 8, GetScreenHeight() / 16, 32, WHITE);
    DrawText(TextFormat("%i", PLAYER_SCORE), (GetScreenWidth() - (GetScreenWidth() / 4)) - 8, GetScreenHeight() / 16, 32, WHITE);
}

void CollisionCheck(Ball * ball, Paddle * paddle) {
    if(CheckCollisionCircleRec(
        Vector2{(float) ball->x, (float) ball->y}, 
        ball->r, 
        Rectangle{(float) paddle->x, (float) paddle->y, (float) paddle->w, (float) paddle->h})) {
            if(ball->s_x < 0) ball->x = paddle->x + paddle->w + ball->r;
            else ball->x = paddle->x - ball->r;
            ball->s_x *= -1;
    }
}

int main()
{
    int m_screen_w = 1280;
    int m_screen_h = 720;

    // Initialize raylib
    InitWindow(m_screen_w, m_screen_h, "Pong");
    SetWindowState(FLAG_VSYNC_HINT);

    // Initialize entities
    Ball ball{ GetScreenWidth() / 2, GetScreenHeight() / 2, 500, 500, 8 };
    Paddle leftPaddle{ GetScreenWidth() / 16,  GetScreenHeight() / 2 - 64};
    Paddle rightPaddle{ GetScreenWidth() - (GetScreenWidth() / 16) - 8,  GetScreenHeight() / 2 - 64};

    // Main game loop
    while (!WindowShouldClose())
    {
        // Draw background
        BeginDrawing();
        UpdateRay();
        UpdateBackground();

        if(ball.x < GetScreenWidth() / 2) CollisionCheck(&ball, &leftPaddle);
        else CollisionCheck(&ball, &rightPaddle);

        leftPaddle.Update();
        rightPaddle.Update();
        ball.Update();

        leftPaddle.Auto(ball.y);
        rightPaddle.Input();

        EndDrawing();
    }

    // Close the window and unload all resources
    CloseWindow();

    return 0;
}
