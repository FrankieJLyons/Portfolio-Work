#include "ball.h"

int Ball::nextId = 0;

////////////
// Public //
////////////
Ball::Ball(): id(nextId++) {
    position = initialPosition;
    radius = 8;
    direction = vm.GetRandomDirection();
    speed = initialSpeed;
    velocity = vm.Scale(direction, speed);
    lastCollisionTime = 0.0;
}

void Ball::Print() {
    std::cout << "\rBall Pos: X: " << position.x << ", Y: " << position.y << "\n";
    std::cout << "\rBall Vel: X: " << velocity.x << ", Y: " << velocity.y << "\n";
    std::cout << "\rBall Dir: X: " << direction.x << ", Y: " << direction.y << "\n";
    std::cout << "\rBall speed: " << speed << "\n";
}

void Ball::Draw() {
    DrawCircle(position.x, position.y, radius, WHITE);
}

void Ball::Update() {        
    position.x += velocity.x * GetFrameTime();
    position.y += velocity.y * GetFrameTime();
    velocity = vm.Scale(direction, speed);
}