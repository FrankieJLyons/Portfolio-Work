#ifndef GAME_H
#define GAME_H

#include <list>
#include <iostream>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include <raylib.h>

#include "ball.h"
#include "collisions.h"
#include "consts.h"
#include "paddle.h"
#include "particle.h"

class Game
{
public:
    Game();
    void Setup();
    void Draw();
    void Update();

private:
    // Set up timer to add new balls
    float addBallInterval = 4.0f; // time in seconds
    float timeSinceLastBall = 0.0f;

    float previousFrameTime = GetTime();

    bool debuging = true;
    bool paused = false;

    Sound soundPause = LoadSound("/Users/frankie/Developer/Portfolio-Work/pong/assets/Pause.ogg");

    void DrawPlayground();
    void DrawBalls();
    void DrawPaddles();
    void DrawScores();
    void DrawSquares();
    void DrawPaused();
    void DrawDebug();

    void AddBall();
    void UpdateBall();
    void UpdatePaddle();

    void Particles();

    list<Ball> balls;
    map<string, Paddle> paddles;
    map<string, int> scores;
    map<string, Rectangle> squares;
    Collisions collisions;
    vector<unique_ptr<ParticleSpawner>> spawners;
};

extern Game game;

#endif