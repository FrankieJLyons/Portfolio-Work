#include <raylib.h>
#include <iostream>

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