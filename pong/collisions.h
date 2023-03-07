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
    ~Collisions() {
        UnloadSound(soundPaddle);
        UnloadSound(soundSquare);
        UnloadSound(soundScore);
        UnloadSound(soundBall);
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

    const float RANDOM_DIRECTION_BUFFER = 0.05f;
    const float MTD_BUFFER = 1.05f;
    const float ELAPSED_TIME_BUFFER = 0.0125f;

    const float BALL_SPEED_UP_FACTOR = 1.05f;
    const float BALL_SLOW_DOWN_FACTOR = 0.95f;
};

#endif