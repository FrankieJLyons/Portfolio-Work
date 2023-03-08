#include "screenmanager.h"

ScreenManager::ScreenManager() {
    current = Screen::ScMenu;
}

void ScreenManager::Draw() {
    switch (current)
    {
    case Screen::ScMenu:
        menu->Draw();
        break;

    case Screen::ScGame:
        game->Draw();
        break;
    
    default:
        break;
    }
}

void ScreenManager::Update() {
    switch (current)
    {
    case Screen::ScMenu:
        menu->Update();
        break;

    case Screen::ScGame:
        game->Update();
        break;
    
    default:
        break;
    }
}

void ScreenManager::ProcessInput() {
    switch (current)
    {
    case Screen::ScMenu:
        menu->ProcessInput();
        if(IsKeyPressed(KEY_ENTER)) ChangeScreen(Screen::ScGame);
        break;

    case Screen::ScGame:
        game->ProcessInput();
        if(IsKeyPressed(KEY_BACKSPACE)) ChangeScreen(Screen::ScMenu);
        break;
    
    default:
        break;
    }
}

void ScreenManager::ChangeScreen(Screen screen) {
    current = screen;

    switch (screen)
    {
    case Screen::ScMenu:
        menu = new Menu();
        break;

    case Screen::ScGame:
        game = new Game();
        break;
    
    default:
        break;
    }
}