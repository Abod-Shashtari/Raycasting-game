#include "TitleScreen.hpp"
#include "raylib.h"
#include "Constants.hpp"


Font titleFont;

TitleScreen::TitleScreen(){
    titleText="THE RAYCASTING GAME";
    gameOverText="-YOU DIED-";
    titleFont= LoadFont("../assets/alagard.png");
    fontSize=titleFont.baseSize*4;
    titleTextSize=MeasureTextEx(titleFont,titleText,fontSize, 2);
    gameOverTextSize=MeasureTextEx(titleFont,gameOverText,fontSize, 2);
}
bool TitleScreen::update(){
    return IsKeyDown(KEY_ENTER);
}

void TitleScreen::render(bool dead){
    ClearBackground(BLACK);
    if(dead)
        DrawTextEx(titleFont,gameOverText, Vector2{static_cast<float>(WINDOW_WIDTH/2.0-gameOverTextSize.x/2),static_cast<float>(WINDOW_HEIGHT/2.0-gameOverTextSize.y/2)},fontSize, 2, WHITE);
    else
        DrawTextEx(titleFont,titleText, Vector2{static_cast<float>(WINDOW_WIDTH/2.0-titleTextSize.x/2),static_cast<float>(WINDOW_HEIGHT/2.0-titleTextSize.y/2)},fontSize, 2, WHITE);

}
