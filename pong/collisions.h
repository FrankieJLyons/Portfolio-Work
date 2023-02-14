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
    Collisions() {
        SetSoundPitch(soundPaddle, 3.0f); 
        SetSoundPitch(soundSquare, 3.0f); 
        SetSoundPitch(soundScore, 3.0f); 
    }
    void BounceBallOffRectangle(Ball * ball, Rectangle rect);
    void BounceBallOffBall(Ball * ball, Ball * nearbyBall);
    void ResolveBallRectCollision(Ball * ball, Vector2 mtd);
    void ResolveBallBallCollision(Ball* ball, Ball* nearbyBall, Vector2 mtd);
    void BallPaddle(Ball * ball, Paddle paddle);
    void BallRect(Ball * ball, Rectangle rect);
    bool BallBorder(Ball * ball);
    void BallBall(Ball * ball, Ball * nearbyBall);

private:
    VectorMath & vm = VectorMath::getInstance();

    // Load the .wav file
    Sound soundPaddle = LoadSound("assets/Square.ogg");
    Sound soundSquare = LoadSound("assets/Paddle.ogg");
    Sound soundScore = LoadSound("assets/Score.ogg");
    Sound soundBall = LoadSound("assets/Ball.ogg");
};

#endif