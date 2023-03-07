#ifndef PADDLE_H
#define PADDLE_H

#include <list>
#include <iostream>
using namespace std;

#include <raylib.h>

#include "ball.h"
#include "vectorMath.h"

class Paddle {
public:
    Vector2 position;
    int w; 
    int h;
    Color color = WHITE;

    Rectangle bounds;
    Vector2 center;

    float speed;
    Vector2 direction;
    Vector2 velocity;

    bool rotated;
    float approachSpeed;
    
    VectorMath* vm;

    Paddle();
    Paddle(Vector2 position, int w, int h, Color color);
    ~Paddle(){}
    void Print();
    void Draw();
    void Update();

    void Auto(list<Ball>& balls, Rectangle rec1, Rectangle rec2);
    void Input();
};

#endif