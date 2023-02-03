#include <raylib.h>
#include <iostream>

#include "vectorMath.h"

class Ball {
    private:
    VectorMath & vm = VectorMath::getInstance();

    void Draw() {
        DrawCircle(position.x, position.y, radius, WHITE);
    }

    public:
    float radius = 4;
    Vector2 position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    Vector2 direction = { rand() % 2 == 0 ? 1.0f : -1.0f, rand() % 2 == 0 ? 1.0f : -1.0f };
    float speed = 200.0f;
    Vector2 velocity = vm.Scale(direction, speed);


    Ball() {}
    Ball(float r): radius{r} {}

    void Print() {
		std::cout << "\rBall Pos: X: " << position.x << ", Y: " << position.y << "\n";
		std::cout << "\rBall Vel: X: " << velocity.x << ", Y: " << velocity.y << "\n";
		std::cout << "\rBall Dir: X: " << direction.x << ", Y: " << direction.y << "\n";
		std::cout << "\rBall speed: " << speed << "\n";
    }

    void Update() {
        Draw();
        
        position.x += velocity.x * GetFrameTime();
        position.y += velocity.y * GetFrameTime();

        velocity = vm.Scale(direction, speed);
    }

    void Reset() {
        position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        velocity = { 0.0f, 0.0f };

        direction = { rand() % 2 == 0 ? 1.0f : -1.0f, rand() % 2 == 0 ? 1.0f : -1.0f };
        velocity = vm.Scale(direction, speed);
    }
};