#include <raylib.h>
#include <iostream>

class Paddle {
    public:
    Vector2 position;
    int w = 16; 
    int h = 128;

    Vector2 center = {position.x + (w / 2), position.y + (h / 2)};
    Rectangle bounds = {position.x, position.y, (float) w, (float) h};

    float speed = 300.0f;
    Vector2 velocity = { speed, speed };

    bool rotated = h < w;
    float autoOffset = 8.0f;

    Color color = WHITE;

    Paddle(Vector2 position, int w, int h): position{position}, w{w}, h{h} {}
    Paddle(Vector2 position, int w, int h, Color color): position{position}, w{w}, h{h}, color{color} {}
        
    void Print() {
		std::cout << "Paddle Pos: X: " << position.x << ", Y: " << position.y << "\n";
		std::cout << "Paddle Vel: X: " << velocity.x << ", Y: " << velocity.y << "\n";
    }

    void Update() {
        Draw();
        CollisionBorder();
        
        center = {position.x + (w / 2), position.y + (h / 2)};
        bounds = {position.x, position.y, (float) w, (float) h};
    }

    void Input() {
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && position.y > 0) position.y -= velocity.y * GetFrameTime();
        if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && position.y < GetScreenHeight() - h) position.y += velocity.y * GetFrameTime();
    }

    void Auto(Ball * ball) {
        center = {position.x + (w / 2), position.y + (h / 2)};

        if(rotated) {
            if (ball->position.x > center.x + autoOffset) position.x += velocity.x * GetFrameTime();
            else if (ball->position.x <= center.x - autoOffset) position.x -= velocity.x * GetFrameTime();
        } else {
            if (ball->position.y > center.y + autoOffset) position.y += velocity.y * GetFrameTime();
            else if (ball->position.y <= center.y - autoOffset) position.y -= velocity.y * GetFrameTime();
        }
    }

    private:
    void Draw() {
        DrawRectangle(position.x, position.y, w, h, WHITE);
    }

    void CollisionBorder() {
        if(rotated) {
            if(position.x + w + velocity.x * GetFrameTime() >= GetScreenWidth()) {
                position.x = GetScreenWidth() - w;
            } else if(position.x - velocity.x * GetFrameTime() <= 0) {
                position.x = 0;
            }
        } else {
            if(position.y + h + velocity.y * GetFrameTime() >= GetScreenHeight()) {
                position.y = GetScreenHeight() - h;
            } else if(position.y - velocity.y * GetFrameTime() <= 0) {
                position.y = 0;
            }
        }
    }
};