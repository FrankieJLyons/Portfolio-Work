#include <raylib.h>
#include <iostream>

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
        position = Vector2Add(position, velocity);
    }

    void Reset() {
        position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        velocity = { 0.0f, 0.0f };

        int randomX = rand() % 2 == 0 ? 1 : -1;
        int randomY = rand() % 2 == 0 ? 1 : -1;
        setVelocity(4.0f, Vector2 { (float) randomX, (float) randomY });
    }

    private:
    void Draw() {
        DrawCircle(position.x, position.y, radius, WHITE);
    }

    Vector2 Vector2Add(Vector2 one, Vector2 two) {
        return {one.x + two.x, one.y + two.y};
    }

    Vector2 Vector2Multiply(Vector2 one, Vector2 two) {
        return {one.x * two.x, one.y * two.y};
    }
};