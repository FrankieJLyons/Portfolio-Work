#ifndef GAME_H
#define GAME_H

#include <list>
#include <iostream>
#include <map>
#include <cmath>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include <raylib.h>

#include "ball.h"
#include "collisions.h"
#include "paddle.h"
#include "quadtree.h"

#include "consts.h"
#include "event.h"
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
    float addBallInterval = 2.0f; // time in seconds
    float timeSinceLastBall = 0.0f;

    float previousFrameTime = GetTime();

    bool debuging = true;
    bool paused = false;
    bool gameOver = false;

    string winner = "Nobody";
    Color winningColor = WHITE;

    Sound soundPause = LoadSound("assets/Pause.ogg");
    Sound soundOut = LoadSound("assets/Out.ogg");
    Sound soundWin = LoadSound("assets/Win.ogg");

    void DrawPlayground();
    void DrawBalls();
    void DrawPaddles();
    void DrawScores();
    void DrawSquares();
    void DrawBlockers();
    void DrawGoal();
    void DrawPaused();
    void DrawDebug();

    void Particles();
    void Goal(Color * goalColor, string name);
    void AddBall();

    void UpdateBall();
    void UpdatePaddle();

    void GameOver();
    void DrawWinScreen();

    list<Ball> balls;
    map<string, Paddle> paddles;
    map<string, int> scores;
    map<string, Rectangle> squares;
    map<string, Color> squareColors;
    map<string, bool> squareColorUpdate;
    Collisions collisions;
    vector<unique_ptr<ParticleSpawner>> spawners;
    map<string, Rectangle> blockers;
    Event<bool> events;
};

#endif