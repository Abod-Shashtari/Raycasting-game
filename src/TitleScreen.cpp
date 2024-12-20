#include "TitleScreen.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "raylib.h"
#include "Constants.hpp"
#include <vector>


Font titleFont;

TitleScreen::TitleScreen(){
    titleText="THE RAYCASTING GAME";
    titleFont= LoadFont("../assets/alagard.png");
    fontSize=titleFont.baseSize*4;
    titleTextSize=MeasureTextEx(titleFont,titleText,fontSize, 2);
}
bool TitleScreen::update(std::vector<GameObject> &eneimes,Player &player,int &score){
    if(IsKeyDown(KEY_ENTER)){
        eneimes.clear();
        player.getPos()->setPosX(400);
        player.getPos()->setPosY(200);
        score=0;
        return IsKeyDown(KEY_ENTER);
    }
    return false;
}

void TitleScreen::render(bool dead,int score,int highScore,bool pause){
    ClearBackground(BLACK);
    if(dead){
        gameOverText=TextFormat("-YOU DIED-");
        gameOverTextSize=MeasureTextEx(titleFont,gameOverText,fontSize, 2);

        gameOverScoreText=TextFormat("score : %d\nHigh score : %d",score,highScore);
        gameOverScoreTextSize=MeasureTextEx(titleFont,gameOverScoreText,fontSize, 2);

        DrawTextEx(titleFont,gameOverText, Vector2{static_cast<float>(WINDOW_WIDTH/2.0-gameOverTextSize.x/2),static_cast<float>(WINDOW_HEIGHT/3.0-gameOverTextSize.y/2)},fontSize, 2, WHITE);
        DrawTextEx(titleFont,gameOverScoreText, Vector2{static_cast<float>(WINDOW_WIDTH/2.0-gameOverScoreTextSize.x/2),static_cast<float>(WINDOW_HEIGHT*2/3.0-gameOverScoreTextSize.y/2)},fontSize, 2, WHITE);
    }else if(pause){
        pauseText=TextFormat("PAUSE");
        pauseTextSize=MeasureTextEx(titleFont,pauseText,fontSize,2);
        DrawTextEx(titleFont,pauseText, Vector2{static_cast<float>(WINDOW_WIDTH/2.0-pauseTextSize.x/2),static_cast<float>(WINDOW_HEIGHT/2.0-pauseTextSize.y/2)},fontSize, 2, WHITE);

    }else{
        DrawTextEx(titleFont,titleText, Vector2{static_cast<float>(WINDOW_WIDTH/2.0-titleTextSize.x/2),static_cast<float>(WINDOW_HEIGHT/2.0-titleTextSize.y/2)},fontSize, 2, WHITE);
    }

}
