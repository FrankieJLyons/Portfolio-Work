#include "paddle.h"

////////////
// Public //
////////////
Paddle::Paddle()
    : position({0, 0}), w(0), h(0), color(WHITE) {
}

Paddle::Paddle(Vector2 position, int w, int h, Color color) : position(position), w(w), h(h), color(color)
{
    vm = &VectorMath::getInstance();

    bounds = {position.x, position.y, (float) w, (float) h};
    center = {position.x + (w / 2), position.y + (h / 2)};

    speed = 300.0f;
    direction = {0.0f, 0.0f};
    velocity = vm->Scale(direction, speed);
    rotated = h < w;
    approachSpeed = 0.1f;
}

void Paddle::Print() {
    std::cout << "Paddle Pos: X: " << position.x << ", Y: " << position.y << "\n";
    std::cout << "Paddle Vel: X: " << velocity.x << ", Y: " << velocity.y << "\n";
}

void Paddle::Draw() {
    DrawRectangleRec(bounds, color);
}

void Paddle::Update() {
    position.x += velocity.x * GetFrameTime();
    position.y += velocity.y * GetFrameTime();
    velocity = vm->Scale(direction, speed);

    bounds = {position.x, position.y, (float) w, (float) h};
    center = {position.x + (w / 2), position.y + (h / 2)};
}

void Paddle::Auto(list<Ball>& balls, Rectangle rec1, Rectangle rec2) {
    // Border Logic
    if(rotated) {
        float newX = position.x + velocity.x * GetFrameTime();
        if(newX + w >= rec2.x || position.x + w >= rec2.x) {
            position.x = rec2.x - w;
            direction.x = 0;
        } else if(newX <= rec1.x + rec1.width || position.x <= rec1.x + rec1.width) {
            position.x = rec1.x + rec1.width;
            direction.x = 0;
        }
    } else {
        float newY = position.y + velocity.y * GetFrameTime();
        if(newY + h >= rec2.y || position.y + h >= rec2.y) {
            position.y = rec2.y - h;
            direction.y = 0;
        } else if(newY <= rec1.y + rec1.height || position.y <= rec1.y + rec1.height) {
            position.y = rec1.y + rec1.height;
            direction.y = 0;
        }
    }

    // Center logic
    if(balls.empty()) {         
        Vector2 restPosition = {(float) GetScreenWidth() / 2 - w / 2, (float) GetScreenHeight() / 2 - h / 2};  
        direction = {0, 0};
        if(rotated && !vm->isWithinRange(position.x, restPosition.x, 0)){
            position.x += (restPosition.x - position.x) * (approachSpeed / 10);
        } else if(!rotated && !vm->isWithinRange(position.y, restPosition.y, 0)){
            position.y += (restPosition.y - position.y) * (approachSpeed / 10);
        }
    } else { // Follow logic
        Ball * ball;

        float shortestDistance = 1000.0f;
        bool ballFound = false;
        for (Ball & b : balls) {
            float d = vm->Length(vm->Subtract(position, b.position));

            bool dangerousDirection = false;
            if (rotated) {
                if (position.y < GetScreenHeight() / 2) { // Top
                    dangerousDirection = b.direction.y < 0; // Up
                } else {
                    dangerousDirection = b.direction.y > 0; // Down
                }
            } else {
                if (position.x < GetScreenWidth() / 2) { // Left
                    dangerousDirection = b.direction.x < 0; // Left
                } else {
                    dangerousDirection = b.direction.x > 0; // Right
                }
            }

            if(d < shortestDistance && dangerousDirection) {
                ball = &b;
                shortestDistance = d;
                ballFound = true;
            }
        }

        if (!ballFound) {
            for (Ball & b : balls) {
                float d = vm->Length(vm->Subtract(position, b.position));
                if (d < shortestDistance) {
                    ball = &b;
                    shortestDistance = d;
                }
            }
        }

        float buffer = ball->radius * 2;
        
        if(rotated) {
            if(vm->isWithinRange(ball->position.x, center.x, (bounds.width / 2))){
                direction.x += (ball->direction.x - direction.x) * approachSpeed;
            }
            else if (ball->position.x < bounds.x) {
                direction.x += (-1.0f - direction.x) * (approachSpeed / 2);
            }
            else if (ball->position.x > bounds.x + bounds.width) {
                direction.x += (1.0f - direction.x) * (approachSpeed / 2);
            }
        } else {
            if(vm->isWithinRange(ball->position.y, center.y, (bounds.height / 2))){
                direction.y += (ball->direction.y - direction.y) * approachSpeed;
            }
            else if (ball->position.y < bounds.y) {   
                direction.y += (-1.0f - direction.y) * (approachSpeed / 2);
            }
            else if (ball->position.y > bounds.y + bounds.height) {
                direction.y += (1.0f - direction.y) * (approachSpeed / 2);
            }
        }
    }
}

void Paddle::Input() {
    if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && position.y > 0) position.y -= velocity.y * GetFrameTime();
    if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && position.y < GetScreenHeight() - h) position.y += velocity.y * GetFrameTime();
}