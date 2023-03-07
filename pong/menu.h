#ifndef MENU_H
#define MENU_H

#include <iostream>

#include <raylib.h>

#include "consts.h"

class Menu {
public:
    Menu();
    ~Menu(){}
    void Draw();
    void Update();
    void ProcessInput();
};

#endif