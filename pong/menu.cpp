#include "menu.h"

////////////
// Public //
////////////
Menu::Menu() {
}

void Menu::Draw() {
    DrawText("PONG^2", HALF_W - MeasureText("PONG^2", 40)/2, HALF_H - 20, 40, WHITE);


    DrawText("Press ENTER to play", HALF_W - MeasureText("Press ENTER to play", 20) / 2, HALF_H + 64, 20, WHITE);
}

void Menu::Update() {

}

void Menu::ProcessInput() {
}