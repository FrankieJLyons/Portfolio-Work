#include <raylib.h>
#include <iostream>

class Paddle {
    private:
    VectorMath & vm = VectorMath::getInstance();

    void Draw() {
        DrawRectangleRec(bounds, color);
    }

    public:
    Vector2 position;
    int w = 16; 
    int h = 128;
    Rectangle bounds = {position.x, position.y, (float) w, (float) h};
    Vector2 center = {position.x + (w / 2), position.y + (h / 2)};

    float speed = 300.0f;
    Vector2 direction = {0.0f, 0.0f};
    Vector2 velocity = vm.Scale(direction, speed);

    bool rotated = h < w;
    Color color = WHITE;
    float approachSpeed = 0.125f;

    Paddle(Vector2 position, int w, int h): position{position}, w{w}, h{h} {}
    Paddle(Vector2 position, int w, int h, Color color): position{position}, w{w}, h{h}, color{color} {}
        
    void Print() {
		std::cout << "Paddle Pos: X: " << position.x << ", Y: " << position.y << "\n";
		std::cout << "Paddle Vel: X: " << velocity.x << ", Y: " << velocity.y << "\n";
    }

    void Update() {
        Draw();

        position.x += velocity.x * GetFrameTime();
        position.y += velocity.y * GetFrameTime();
        velocity = vm.Scale(direction, speed);

        bounds = {position.x, position.y, (float) w, (float) h};
        center = {position.x + (w / 2), position.y + (h / 2)};
    }

    void Input() {
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && position.y > 0) position.y -= velocity.y * GetFrameTime();
        if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && position.y < GetScreenHeight() - h) position.y += velocity.y * GetFrameTime();
    }

    void Auto(Ball * ball, Rectangle rec1, Rectangle rec2) {
        float buffer = ball->radius * 2;
        if(rotated) {
            if(ball->position.x >= bounds.x + buffer && ball->position.x <= bounds.x + bounds.width - buffer) {
                direction.x += (ball->direction.x - direction.x) * approachSpeed;
            }
            else if (ball->position.x <= bounds.x) {
                direction.x += (-1.0f - direction.x) * approachSpeed;
            }
            else if (ball->position.x >= bounds.x + bounds.width) {
                direction.x += (1.0f - direction.x) * approachSpeed;
            }

            float newX = position.x + velocity.x * GetFrameTime();
            if(newX + w >= rec2.x) {
                position.x = rec2.x - w;
            } else if(newX <= rec1.x + rec1.width) {
                position.x = rec1.x + rec1.width;
            }
        } else {
            if(ball->position.y >= bounds.y + buffer && ball->position.y <= bounds.y + bounds.height - buffer) {    
                direction.y += (ball->direction.y - direction.y) * approachSpeed;
            }
            else if (ball->position.y <= bounds.y) {   
                direction.y += (-1.0f - direction.y) * approachSpeed;
            }
            else if (ball->position.y >= bounds.y + bounds.height) {
                direction.y += (1.0f - direction.y) * approachSpeed;
            }

            float newY = position.y + velocity.y * GetFrameTime();
            if(newY + h >= rec2.y) {
                position.y = rec2.y - h;
            } else if(newY <= rec1.y + rec1.height) {
                position.y = rec1.y + rec1.height;
            }
        }
    }
};