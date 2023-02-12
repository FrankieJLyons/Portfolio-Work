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
    void ResolveBallRectCollision(Ball * ball, Vector2 mtd);
    void BallPaddle(Ball * ball, Paddle paddle);
    void BallRect(Ball * ball, Rectangle rect);
    bool BallBorder(Ball * ball);

private:
    VectorMath & vm = VectorMath::getInstance();

    // Load the .wav file
    Sound soundPaddle = LoadSound("/Users/frankie/Developer/Portfolio-Work/pong/assets/Square.ogg");
    Sound soundSquare = LoadSound("/Users/frankie/Developer/Portfolio-Work/pong/assets/Paddle.ogg");
    Sound soundScore = LoadSound("/Users/frankie/Developer/Portfolio-Work/pong/assets/Score.ogg");
};

#endif