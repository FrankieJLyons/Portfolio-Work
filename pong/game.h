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
    ~Game();
    void Setup();
    void Draw();
    void Update();
    void ProcessInput();
    void ResetGame();

private:
    // Set up timer to add new balls
    float addBallInterval; // time in seconds
    float timeSinceLastBall;

    float previousFrameTime;

    bool debuging;
    bool paused;
    bool gameOver;

    string winner;
    Color winningColor;

    int eventCount;

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
    vector<ParticleSpawner> spawners;
    map<string, Rectangle> blockers;
    Event<bool> events;

    const int INIT_SCORE = 1;
};

#endif