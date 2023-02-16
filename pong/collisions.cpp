#include "collisions.h"

// Ball / Rectangle bounce 
void Collisions::BounceBallOffRectangle(Ball * ball, Rectangle rect) {
    // Going Up and Left
    if(ball->velocity.y < 0 && ball->velocity.x < 0) {
        // Bottom Right
        if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Back the way it came
            ball->direction.x *= -1;
            ball->direction.y *= -1;
        }
        // Top Right
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        }
        // Bottom Left
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        } 
        // Right 
        else if (vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Bottom 
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
    }

    // Going Up and Right
    else if(ball->velocity.y < 0 && ball->velocity.x > 0) {
        // Bottom Left
        if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Back the way it came
            ball->direction.x *= -1;
            ball->direction.y *= -1;
        } 
        // Top Left
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        }
        // Bottom Right
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
        // Left 
        else if (vm.isWithinRange(ball->position.x, rect.x , ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Bottom 
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
    }

    // Going Down and Right 
    else if(ball->velocity.y > 0 && ball->velocity.x > 0) {
        // Top Left
        if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Back the way it came
            ball->direction.x *= -1;
            ball->direction.y *= -1;
        }
        // Top Right
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
        // Bottom Left
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Left 
        else if (vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Top 
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
    }

    // Going Down and Left
    else if(ball->velocity.y > 0 && ball->velocity.x < 0) {
        // Top Right
        if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Back the way it came
            ball->direction.x *= -1;
            ball->direction.y *= -1;
        }
        // Top Left
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
        // Bottom Right
        else if (vm.isWithinRange(ball->position.y, rect.y + rect.height, ball->radius) 
        && vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        }
        // Right 
        else if (vm.isWithinRange(ball->position.x, rect.x + rect.width, ball->radius)) {
            // Reflect X
            ball->direction.x *= -1;
        } 
        // Top 
        else if (vm.isWithinRange(ball->position.y, rect.y, ball->radius)) {
            // Reflect Y
            ball->direction.y *= -1;
        }
    }
}

void Collisions::ResolveBallRectCollision (Ball * ball, Vector2 mtd) {
    ball->direction = vm.Add(ball->direction, vm.Scale(vm.GetRandomDirection(), 0.05f));
    if(ball->speed <= ball->maxSpeed) ball->speed *= 1.05f;
    ball->velocity = vm.Scale(ball->direction, ball->speed);
    ball->position = vm.Add(ball->position, vm.Scale(mtd, 1.025f));
}

void Collisions::BallPaddle(Ball * ball, Paddle paddle) {
    static double lastCollisionTime = 0.0;
    double currentTime = GetTime();
    double elapsedTime = currentTime - lastCollisionTime;
    Rectangle rect = paddle.bounds;
    if(CheckCollisionCircleRec(
        ball->position, 
        ball->radius, 
        rect) && elapsedTime > 0.025) {
        lastCollisionTime = currentTime;
        Vector2 next = {paddle.position.x + paddle.velocity.x * GetFrameTime(), paddle.position.y + paddle.velocity.y * GetFrameTime()};
        Vector2 mtd = vm.GetMinimumMovingTranslation(ball->position, ball->radius, rect, next);
        BounceBallOffRectangle(ball, rect);
        ResolveBallRectCollision(ball, mtd);
        PlaySound(soundPaddle); 
    }
}

void Collisions::BallRect(Ball * ball, Rectangle rect) {
    if(CheckCollisionCircleRec(
        ball->position, 
        ball->radius, 
        rect)) {   
        Vector2 mtd = vm.GetMinimumTranslation(ball->position, ball->radius, rect);
        BounceBallOffRectangle(ball, rect);
        ResolveBallRectCollision(ball, mtd);
        PlaySound(soundSquare);
    }
}

bool Collisions::BallBorder(Ball * ball) {
    int buffer = ball->radius * 4.0f;
    if(!CheckCollisionCircleRec(
    ball->position, 
    buffer, 
    Rectangle {PLAYGROUND_X, 0, SCREEN_H, SCREEN_H})) {  
        PlaySound(soundScore);
        return true;
    }
    return false;
}

void Collisions::BallBall(Ball * ball, Ball * nearbyBall) {
    if(CheckCollisionCircles(
        ball->position, 
        ball->radius, 
        nearbyBall->position, 
        nearbyBall->radius)) {   
        Vector2 mtd = vm.GetMinimumBallTranslation(ball->position, ball->radius, nearbyBall->position, nearbyBall->radius);
        ResolveBallBallCollision(ball, nearbyBall, mtd);
        BounceBallOffBall(ball, nearbyBall);
        PlaySound(soundBall);
    }
}

void Collisions::ResolveBallBallCollision(Ball* ball, Ball* nearbyBall, Vector2 mtd) {
    float combinedRadius = ball->radius + nearbyBall->radius;
    float overlap = combinedRadius - vm.Length(mtd);
    if (overlap > 0) {
        Vector2 normal = vm.Normalize(mtd);
        Vector2 correction = vm.Scale(normal, overlap);
        ball->position = vm.Add(ball->position, vm.Scale(correction, (-ball->mass / (ball->mass + nearbyBall->mass)) * 1.025));
        nearbyBall->position = vm.Add(nearbyBall->position, vm.Scale(correction, (nearbyBall->mass / (ball->mass + nearbyBall->mass)) * 1.025));
    }
}

void Collisions::BounceBallOffBall(Ball* ball, Ball* nearbyBall) {
    Vector2 normal = vm.Subtract(nearbyBall->position, ball->position);
    ball->velocity = vm.Scale(ball->direction, ball->speed);
    nearbyBall->velocity = vm.Scale(nearbyBall->direction, nearbyBall->speed);
    float dot1 = vm.DotProduct(ball->velocity, normal);
    float dot2 = vm.DotProduct(nearbyBall->velocity, normal);
    Vector2 reflection1 = vm.Subtract(ball->velocity, vm.Scale(normal, 2 * dot1));
    Vector2 reflection2 = vm.Subtract(nearbyBall->velocity, vm.Scale(normal, 2 * dot2));
    ball->direction = vm.Normalize(reflection1);
    nearbyBall->direction = vm.Normalize(reflection2);
}