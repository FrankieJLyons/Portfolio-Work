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
    float approachSpeed = 0.1f;

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

    void Auto(Ball * balls[], Rectangle rec1, Rectangle rec2) {

        Ball ball;
        float shortestDistance = 1000.0f;
        for (int i = 0; i < balls->length; i++) {
            float d = vm.Length(Subtract(ball[i]->position, center));
            if(d <= shortestDistance) {
                shortestDistance = d;
                ball = ball[i];
            }
        }

        float buffer = ball->radius * 2;
        if(rotated) {
            float newX = position.x + velocity.x * GetFrameTime();
            if(newX + w >= rec2.x || position.x + w >= rec2.x) {
                position.x = rec2.x - w;
                direction.x = 0;
            } else if(newX <= rec1.x + rec1.width || position.x <= rec1.x + rec1.width) {
                position.x = rec1.x + rec1.width;
                direction.x = 0;
            }

            if(vm.isWithinRange(ball->position.x, center.x, (bounds.width / 2))){
                direction.x += (ball->direction.x - direction.x) * approachSpeed;
            }
            else if (ball->position.x < bounds.x) {
                direction.x += (-1.0f - direction.x) * (approachSpeed / 2);
            }
            else if (ball->position.x > bounds.x + bounds.width) {
                direction.x += (1.0f - direction.x) * (approachSpeed / 2);
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

            if(vm.isWithinRange(ball->position.y, center.y, (bounds.height / 2))){
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
};