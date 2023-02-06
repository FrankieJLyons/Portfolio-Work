#include <raylib.h>
#include <iostream>

#include "vectorMath.h"

class Ball {
    private:
    VectorMath & vm = VectorMath::getInstance();

    // Vector2 initialPosition = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    Vector2 initialPosition = { GetScreenWidth() / 3.0f + 48, GetScreenHeight() / 3.0f };
    float initialSpeed = 300.0f;

    void Draw() {
        DrawCircle(position.x, position.y, radius, WHITE);
    }

    public:
    float radius = 8;

    Vector2 position = initialPosition;
    Vector2 previousPosition = initialPosition;

    // Vector2 direction = vm.GetRandomDirection();
    Vector2 direction = Vector2 { 1.0f, -1.0f};
    float speed = initialSpeed;
    const float maxSpeed = speed * 2;

    Vector2 velocity = vm.Scale(direction, speed);


    Ball() {}
    Ball(float r): radius{r} {}

    void Print() {
		std::cout << "\rBall Pos: X: " << position.x << ", Y: " << position.y << "\n";
		std::cout << "\rBall Vel: X: " << velocity.x << ", Y: " << velocity.y << "\n";
		std::cout << "\rBall Dir: X: " << direction.x << ", Y: " << direction.y << "\n";
		std::cout << "\rBall speed: " << speed << "\n";

        static int count = 0;
        count++;
        std::cout << "\rCollisions: " << count << "\n";
    }

    void Update() {
        Draw();
        
        previousPosition = position;
        position.x += velocity.x * GetFrameTime();
        position.y += velocity.y * GetFrameTime();

        velocity = vm.Scale(direction, speed);
    }

    void Reset() {
        position = initialPosition;
        // direction = vm.GetRandomDirection();
        Vector2 direction = Vector2 { -1.0f, -1.0f};
        speed = initialSpeed;
        velocity = vm.Scale(direction, speed);
    }
};