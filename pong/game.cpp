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
    DrawSquares();
    DrawBalls();
    DrawPaddles();
    DrawScores();
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
    paddles["TOP"].Draw();
    paddles["BOTTOM"].Draw();
    paddles["LEFT"].Draw();
    paddles["RIGHT"].Draw();
}

void Game::DrawScores() {
    DrawText(TextFormat("%i", scores["TOP"]), PLAYGROUND_W - TEXT_POS, TEXT_OFFSET, FONT_SIZE, GREEN);
    DrawText(TextFormat("%i", scores["BOTTOM"]), PLAYGROUND_X + TEXT_OFFSET, SCREEN_H - TEXT_POS, FONT_SIZE, GOLD);
    DrawText(TextFormat("%i", scores["LEFT"]), PLAYGROUND_X + TEXT_OFFSET, TEXT_OFFSET, FONT_SIZE, RED);
    DrawText(TextFormat("%i", scores["RIGHT"]), PLAYGROUND_W - TEXT_POS, SCREEN_H - TEXT_POS, FONT_SIZE, BLUE);
}

void Game::DrawSquares() {
    DrawRectangleRec(squares["TOP_LEFT"], WHITE);
    DrawRectangleRec(squares["BOTTOM_LEFT"], WHITE);
    DrawRectangleRec(squares["TOP_RIGHT"], WHITE);
    DrawRectangleRec(squares["BOTTOM_RIGHT"], WHITE);
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
        if(ball.position.y < HALF_H) collisions.BallPaddle(&ball, paddles["TOP"]);
        else collisions.BallPaddle(&ball, paddles["BOTTOM"]);
        if(ball.position.x < HALF_W) collisions.BallPaddle(&ball, paddles["LEFT"]);
        else collisions.BallPaddle(&ball, paddles["RIGHT"]);

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
            if (ball.position.y < 0) {
                scores["TOP"]--;
            } else if (ball.position.y > SCREEN_H){
                scores["BOTTOM"]--;
            } else if (ball.position.x < PLAYGROUND_X) {
                scores["LEFT"]--;
            } else if (ball.position.x > PLAYGROUND_W){
                scores["RIGHT"]--;
            }
            toRemove.push_back(ball);
        }
    }

    for (Ball &ball : toRemove) {
        balls.remove(ball);
    }
}

void Game::UpdatePaddle() {
    paddles["TOP"].Update();
    paddles["BOTTOM"].Update();
    paddles["LEFT"].Update();
    paddles["RIGHT"].Update();

    // AI Movement
    paddles["TOP"].Auto(balls, squares["TOP_LEFT"], squares["TOP_RIGHT"]);
    paddles["BOTTOM"].Auto(balls, squares["BOTTOM_LEFT"], squares["BOTTOM_RIGHT"]);
    paddles["LEFT"].Auto(balls, squares["TOP_LEFT"], squares["BOTTOM_LEFT"]);
    paddles["RIGHT"].Auto(balls, squares["TOP_RIGHT"], squares["BOTTOM_RIGHT"]);
    
    // // Player Movement
    // rightPaddle.Input();
}