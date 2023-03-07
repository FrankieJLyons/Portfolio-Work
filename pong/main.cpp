#include <time.h>
#include <cmath>
using namespace std;

#include <raylib.h>

#include "screenmanager.h"

int main()
{
    // Initialize random seed
    srand((unsigned int)time(NULL));

    InitAudioDevice();

    ScreenManager sm = ScreenManager();

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        sm.Update();
        sm.Draw();
        sm.ProcessInput();

        EndDrawing();
    }

    CloseAudioDevice(); 

    // Close the window and unload all resources
    CloseWindow();

    return 0;
}
