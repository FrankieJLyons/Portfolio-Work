#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <iostream>

#include <raylib.h>

#include "consts.h"
#include "game.h"
#include "menu.h"

class ScreenManager {
public:
    ScreenManager();
    ~ScreenManager(){}
    void Draw();
    void Update();
    void ProcessInput();

    void ChangeScreen(Screen screen);

private:
    Screen current;

    Game * game;
    Menu * menu;
};

#endif