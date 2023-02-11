#include <time.h>
#include <cmath>
using namespace std;

#include <raylib.h>

#include "game.h"

int main()
{
    // Initialize random seed
    srand((unsigned int)time(NULL));

    Game game = Game();

    game.Setup();

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        game.Update();
        game.Draw();

        EndDrawing();
    }

    // Close the window and unload all resources
    CloseWindow();

    return 0;
}
