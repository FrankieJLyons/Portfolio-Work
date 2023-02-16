#include "game.h"

////////////
// Public //
////////////
Game::Game() {
    balls.push_back(Ball());

    paddles["TOP"] = Paddle { {(float) HALF_W - LONG_SIDE / 2, 0 }, LONG_SIDE, SHORT_SIDE, GREEN };
    paddles["BOTTOM"] = Paddle { {(float) HALF_W - LONG_SIDE / 2, (float) SCREEN_H - SHORT_SIDE}, LONG_SIDE, SHORT_SIDE, GOLD };
    paddles["LEFT"] = Paddle { {(float) PLAYGROUND_X, (float) HALF_H - LONG_SIDE / 2}, SHORT_SIDE, LONG_SIDE, RED};
    paddles["RIGHT"] = Paddle { {(float) PLAYGROUND_W - SHORT_SIDE, (float) HALF_H - LONG_SIDE / 2}, SHORT_SIDE, LONG_SIDE, BLUE };

    scores["TOP"] = 10;
    scores["BOTTOM"] = 10;
    scores["LEFT"] = 10;
    scores["RIGHT"] = 10;

    squares["TOP_LEFT"] = Rectangle { (float) PLAYGROUND_X, 0, SQUARE, SQUARE };
    squares["BOTTOM_LEFT"] = Rectangle { (float) PLAYGROUND_X, SCREEN_H - SQUARE, SQUARE, SQUARE };
    squares["TOP_RIGHT"] = Rectangle { (float) PLAYGROUND_W - SQUARE, 0, SQUARE, SQUARE };
    squares["BOTTOM_RIGHT"] = Rectangle { (float) PLAYGROUND_W - SQUARE, SCREEN_H - SQUARE, SQUARE, SQUARE};

    squareColors["TOP_LEFT"] = WHITE;
    squareColors["BOTTOM_LEFT"] = WHITE;
    squareColors["TOP_RIGHT"] = WHITE;
    squareColors["BOTTOM_RIGHT"] = WHITE;

    blockers["TOP"] = Rectangle { (float) PLAYGROUND_X, 0, PLAYGROUND_W, BLOCKER };
    blockers["BOTTOM"] = Rectangle { (float) PLAYGROUND_X, SCREEN_H - BLOCKER, PLAYGROUND_W, BLOCKER };
    blockers["LEFT"] = Rectangle { (float) PLAYGROUND_X, 0, BLOCKER, SCREEN_H };
    blockers["RIGHT"] = Rectangle { (float) PLAYGROUND_W  - BLOCKER, 0, BLOCKER, SCREEN_H };
}

void Game::Setup()
{
    // Initialize raylib
    InitWindow(SCREEN_W, SCREEN_H, "Pong");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(TARGET_FPS);
}

void Game::Draw()
{
    DrawPlayground();
    DrawBlockers();
    DrawSquares();
    DrawBalls();
    DrawPaddles();
    DrawScores();
    DrawGoal();
    DrawPaused();
    DrawDebug();
}

void Game::Update()
{
    if(!paused){ 
        AddBall();
        UpdateBall();
        UpdatePaddle();
    }
}

/////////////
// Private //
/////////////
void Game::DrawPlayground() {
    DrawRectangleLines(PLAYGROUND_X, 0, SCREEN_H, SCREEN_H, WHITE); // Border
    DrawLine(HALF_W, 0, HALF_W, SCREEN_H, WHITE); // Vert divider
    DrawLine(PLAYGROUND_X, HALF_H, PLAYGROUND_W, HALF_H, WHITE); // Hor divider
}

void Game::DrawBalls() {
    for (Ball & ball : balls) {
        ball.Draw();
    }
}

void Game::DrawPaddles() {
    if(scores["TOP"] > 0) paddles["TOP"].Draw();
    if(scores["BOTTOM"] > 0) paddles["BOTTOM"].Draw();
    if(scores["LEFT"] > 0) paddles["LEFT"].Draw();
    if(scores["RIGHT"] > 0) paddles["RIGHT"].Draw();
}

void Game::DrawScores() {
    DrawText(TextFormat("%i", scores["TOP"]), PLAYGROUND_W - TEXT_POS, TEXT_OFFSET, FONT_SIZE, GREEN);
    DrawText(TextFormat("%i", scores["BOTTOM"]), PLAYGROUND_X + TEXT_OFFSET, SCREEN_H - TEXT_POS, FONT_SIZE, GOLD);
    DrawText(TextFormat("%i", scores["LEFT"]), PLAYGROUND_X + TEXT_OFFSET, TEXT_OFFSET, FONT_SIZE, RED);
    DrawText(TextFormat("%i", scores["RIGHT"]), PLAYGROUND_W - TEXT_POS, SCREEN_H - TEXT_POS, FONT_SIZE, BLUE);
}

void Game::DrawSquares() {
    DrawRectangleRec(squares["TOP_LEFT"], squareColors["TOP_LEFT"]);
    DrawRectangleRec(squares["BOTTOM_LEFT"], squareColors["BOTTOM_LEFT"]);
    DrawRectangleRec(squares["TOP_RIGHT"], squareColors["TOP_RIGHT"]);
    DrawRectangleRec(squares["BOTTOM_RIGHT"], squareColors["BOTTOM_RIGHT"]);
}

void Game::DrawBlockers() {
    if(scores["TOP"] <= 0) DrawRectangleRec(blockers["TOP"], WHITE);
    if(scores["BOTTOM"] <= 0) DrawRectangleRec(blockers["BOTTOM"], WHITE);
    if(scores["LEFT"] <= 0) DrawRectangleRec(blockers["LEFT"], WHITE);
    if(scores["RIGHT"] <= 0) DrawRectangleRec(blockers["RIGHT"], WHITE);
}

void Game::DrawGoal() {
    if(squareColorUpdate["GREEN"]) {
        Goal(&squareColors["TOP_RIGHT"], "GREEN");
    }
    if(squareColorUpdate["GOLD"]) {
        Goal(&squareColors["BOTTOM_LEFT"], "GOLD");
    }
    if(squareColorUpdate["RED"]) {
        Goal(&squareColors["TOP_LEFT"], "RED");
    }
    if(squareColorUpdate["BLUE"]) {
        Goal(&squareColors["BOTTOM_RIGHT"], "BLUE");
    }

    if(squareColorUpdate["GREEN"] 
        || squareColorUpdate["GOLD"] 
        || squareColorUpdate["RED"] 
        || squareColorUpdate["BLUE"]){
        Particles();
    }
}

void Game::DrawPaused() {
    if(IsKeyPressed(KEY_P)) { 
        paused = !paused; 
        PlaySound(soundPause);
    }
    if(paused) {
        DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.5f));
        DrawText("PAUSED", HALF_W - MeasureText("PAUSED", 40)/2, HALF_H - 20, 40, WHITE);
    }
}

void Game::DrawDebug() {
    if(IsKeyPressed(KEY_F)) debuging = !debuging;
    if(debuging) {
        DrawFPS(4, 4);
        DrawText(TextFormat("X: %i", (int) balls.front().position.x), 8, 32, 16, WHITE);
        DrawText(TextFormat("Y: %i", (int) balls.front().position.y), 8, 48, 16, WHITE);
        DrawText(TextFormat("S: %i", (int) balls.front().speed), 8, 64, 16, WHITE);
        for (Ball & ball : balls) {
            DrawLine(ball.position.x, ball.position.y,  paddles["TOP"].center.x,  paddles["TOP"].center.y, GREEN);
            DrawLine(ball.position.x, ball.position.y,  paddles["BOTTOM"].center.x,  paddles["BOTTOM"].center.y, GOLD);
            DrawLine(ball.position.x, ball.position.y,  paddles["LEFT"].center.x,  paddles["LEFT"].center.y, RED);
            DrawLine(ball.position.x, ball.position.y, paddles["RIGHT"].center.x, paddles["RIGHT"].center.y, BLUE);
        }
    }
}


void Game::Particles()
{
    float currentFrameTime = GetTime();
    float deltaTime = currentFrameTime - previousFrameTime;
    previousFrameTime = currentFrameTime;

    for (int i = 0; i < spawners.size(); i++) {
        spawners[i]->Update(deltaTime);
        spawners[i]->Draw();
        if (spawners[i]->particles.empty()) {
            spawners.erase(spawners.begin() + i);
            i--;
        }
    }
}

void Game::Goal(Color * goalColor, string name)
{
    if (goalColor->r == WHITE.r 
        && goalColor->g == WHITE.g 
        && goalColor->b == WHITE.b
        && goalColor->a == WHITE.a) {
        squareColorUpdate[name] = false;
    } else {
        float factor = GetFrameTime() * 8;
        goalColor->r += (WHITE.r - goalColor->r) * factor;
        goalColor->g += (WHITE.g - goalColor->g) * factor;
        goalColor->b += (WHITE.b - goalColor->b) * factor;
    }
}

void Game::AddBall() {
    // Add a new ball every `addBallInterval` seconds
    timeSinceLastBall += GetFrameTime();
    if (timeSinceLastBall >= addBallInterval) {
        timeSinceLastBall = 0.0f;
        balls.push_back(Ball());
    }
}

void Game::UpdateBall() {
    std::list<Ball> toRemove;

    for (Ball &ball : balls) {
        ball.Update();
        
        // Check paddle collision based on position
        if(ball.position.y < HALF_H) {
            if(scores["TOP"] > 0) collisions.BallPaddle(&ball, paddles["TOP"]);
            if(scores["TOP"] <= 0) collisions.BallRect(&ball, blockers["TOP"]);
        }
        else {
            if(scores["BOTTOM"] > 0) collisions.BallPaddle(&ball, paddles["BOTTOM"]);
            if(scores["BOTTOM"] <= 0) collisions.BallRect(&ball, blockers["BOTTOM"]);
        }
        if(ball.position.x < HALF_W) {
            if(scores["LEFT"] > 0) collisions.BallPaddle(&ball, paddles["LEFT"]);
            if(scores["LEFT"] <= 0) collisions.BallRect(&ball, blockers["LEFT"]);
        }
        else {
            if(scores["RIGHT"] > 0) collisions.BallPaddle(&ball, paddles["RIGHT"]);
            if(scores["RIGHT"] <= 0) collisions.BallRect(&ball, blockers["RIGHT"]);
        }

        // Check square collision based on position
        if(ball.position.x < HALF_W) {
            if(ball.position.y < HALF_H) collisions.BallRect(&ball,  squares["TOP_LEFT"]);
            else collisions.BallRect(&ball,  squares["BOTTOM_LEFT"]);
        } else {
            if(ball.position.y < HALF_H) collisions.BallRect(&ball,  squares["TOP_RIGHT"]);
            else collisions.BallRect(&ball,  squares["BOTTOM_RIGHT"]);
        }

        // Check if call has left the playground
        if(collisions.BallBorder(&ball)){
            Color color = WHITE;
            if (ball.position.y <= 0) {
                scores["TOP"]--;
                color = GREEN;
                squareColors["TOP_RIGHT"] = color;
                squareColorUpdate["GREEN"] = true;
            } else if (ball.position.y > SCREEN_H){
                scores["BOTTOM"]--;
                color = GOLD;
                squareColors["BOTTOM_LEFT"] = color;
                squareColorUpdate["GOLD"] = true;
            } else if (ball.position.x < PLAYGROUND_X) {
                scores["LEFT"]--;
                color = RED;
                squareColors["TOP_LEFT"] = color;
                squareColorUpdate["RED"] = true;
            } else if (ball.position.x > PLAYGROUND_W){
                scores["RIGHT"]--;
                color = BLUE;
                squareColors["BOTTOM_RIGHT"] = color;
                squareColorUpdate["BLUE"] = true;
            }
            toRemove.push_back(ball);
            spawners.push_back(std::unique_ptr<ParticleSpawner>(new ParticleSpawner(ball.position, color)));   
        }
    }

    for (Ball &ball : toRemove) {
        balls.remove(ball);
    }

    // // Create or update the Quadtree with the current set of balls
    // Quadtree quadtree = {0, {PLAYGROUND_X, 0, PLAYGROUND_W, SCREEN_H}};
    // for (Ball &ball : balls) {
    //     quadtree.Insert(&ball);

    //     // Check ball-to-ball collisions by using the Quadtree
    //     std::vector<Ball*> nearbyBalls;
    //     quadtree.Query(nearbyBalls, ball.position.x - ball.radius, ball.position.y - ball.radius,
    //                 ball.position.x + ball.radius, ball.position.y + ball.radius);
    //     for (Ball* nearbyBall : nearbyBalls) {
    //         if (&ball != nearbyBall) {
    //             collisions.BallBall(&ball, nearbyBall);
    //         }
    //     }   
    // }

    std::set<std::pair<Ball*, Ball*>> comparedPairs;
    std::set<std::pair<Ball*, Ball*>> collidedPairs;
    auto it1 = balls.begin();
    while (it1 != balls.end()) {
        auto it2 = std::next(it1);
        while (it2 != balls.end()) {
            Ball& ball = *it1;
            Ball& ball2 = *it2;
            const auto pair = std::make_pair(&ball, &ball2);
            if (comparedPairs.count(pair) == 0) {
                if (collidedPairs.count(pair) == 0 && collidedPairs.count(std::make_pair(&ball2, &ball)) == 0) {
                    collisions.BallBall(&ball, &ball2);
                    collidedPairs.insert(pair);
                }
                comparedPairs.insert(pair);
            }
            ++it2;
        }
        ++it1;
    }
}

void Game::UpdatePaddle() {
    if(scores["TOP"] > 0) {
        paddles["TOP"].Update();
        paddles["TOP"].Auto(balls, squares["TOP_LEFT"], squares["TOP_RIGHT"]);
    }

    if(scores["BOTTOM"] > 0) {
        paddles["BOTTOM"].Update();
        paddles["BOTTOM"].Auto(balls, squares["BOTTOM_LEFT"], squares["BOTTOM_RIGHT"]);
    }

    if(scores["LEFT"] > 0) {
        paddles["LEFT"].Update();
        paddles["LEFT"].Auto(balls, squares["TOP_LEFT"], squares["BOTTOM_LEFT"]);
    }

    if(scores["RIGHT"] > 0) {
        paddles["RIGHT"].Update();
        paddles["RIGHT"].Auto(balls, squares["TOP_RIGHT"], squares["BOTTOM_RIGHT"]);
    }

    // // Player Movement
    // rightPaddle.Input();
}