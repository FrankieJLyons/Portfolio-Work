#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <iostream>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include <raylib.h>

#include "ball.h"
#include "consts.h"
#include "paddle.h"

class Collisions
{
public:
    Collisions() {}
    void BounceBallOffRectangle(Ball * ball, Rectangle rect);
    void ResolveBallRectCollision(Ball * ball, Vector2 mtd);
    void BallPaddle(Ball * ball, Paddle paddle);
    void BallRect(Ball * ball, Rectangle rect);
    bool BallBorder(Ball * ball);

private:
    VectorMath & vm = VectorMath::getInstance();
};

#endif