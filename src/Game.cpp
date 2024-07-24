#include "Game.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Vector2D.hpp"
#include "raylib.h"
#include "Constants.hpp"
#include "RaycastEngine.hpp"
#include "TitleScreen.hpp"
#include <fstream>
#include <random>
#include <string>
#include <vector>

TitleScreen* titleScreen;
Player* player;
std::vector<GameObject> enemies;

float spwanTimer=0;

float const FLOOR_TIME=3.0;
float floorTimer=FLOOR_TIME;

int playerOldMapPosX;
int playerOldMapPosY;

std::random_device rd;
std::uniform_int_distribution<int> dist(0,3);
std::uniform_real_distribution<float> distTime(ALIEN_MIN_SPAWN_TIME[0],ALIEN_MAX_SPAWN_TIME[0]);

Font font;
int score=0;

Raycast* raycast;
Map myMap;
bool gameStarted=false;


int getHighScore(){
    std::ifstream readFile("../assets/highScore.txt");
    std::string score;
    readFile>>score;
    readFile.close();
    return std::stoi(score);
}

void setHightScore(int score){
    std::ofstream writeFile("../assets/highScore.txt");
    writeFile<<score;
    writeFile.close();
}

void generateEnemies(std::vector<GameObject> &enemies,float deltaTime){
    //alien position
    int randPos=dist(rd);
    //alien spawn
    spwanTimer-=deltaTime;
    //alien speed
    float speed;

    if(score<15)
        speed=ALIEN_SPEED[0];
    else if(score>15 && score<25)
        speed=ALIEN_SPEED[1];
    else if(score>25)
        speed=ALIEN_SPEED[2];

    if(spwanTimer<=0){
        enemies.push_back(*new GameObject(SPWANING_POS[randPos], 1));
        spwanTimer=distTime(rd);
    }

}

//if player stays on the same block for 3 sec he dies
void hotFloor(float deltaTime, bool& dead){
    int PlayerMapPosX=player->getPos()->getPosX()/64.0;
    int PlayerMapPosY=player->getPos()->getPosY()/64.0;
    if(PlayerMapPosX==playerOldMapPosX && PlayerMapPosY==playerOldMapPosY){
        floorTimer-=deltaTime;
        if(floorTimer<=0){
            dead=true;
            floorTimer=FLOOR_TIME;
        }
    }else{
        floorTimer=FLOOR_TIME;
        playerOldMapPosX=player->getPos()->getPosX()/64.0;
        playerOldMapPosY=player->getPos()->getPosY()/64.0;
    }
}

void Game::init(char* title,int width,int height){
    InitWindow(width, height,title);
    titleScreen=new TitleScreen();
    player=new Player(400,200,3);
    playerOldMapPosX=player->getPos()->getPosX()/64.0;
    playerOldMapPosY=player->getPos()->getPosY()/64.0;
    raycast=new Raycast();
    font = LoadFont("../assets/alagard.png");
}

void Game::update(float deltaTime){
    if(!gameStarted){
        gameStarted=titleScreen->update(enemies,*player,score);
    }
    if(!dead){
        if (gameStarted){
            //pause the game
            if(IsKeyPressed(KEY_ESCAPE)){
                pause=(pause)?false:true;
            }
            if(!pause){
                player->update(myMap,enemies,score);
                hotFloor(deltaTime,dead);
                generateEnemies(enemies, deltaTime);

                if(score>10 && score<30)
                    distTime=std::uniform_real_distribution<float>(ALIEN_MIN_SPAWN_TIME[1],ALIEN_MAX_SPAWN_TIME[1]);
                else if(score>30)
                    distTime=std::uniform_real_distribution<float>(ALIEN_MIN_SPAWN_TIME[2],ALIEN_MAX_SPAWN_TIME[2]);

                if (dead) return;

                for(GameObject &e : enemies){
                    dead=e.update(*player->getPos(),myMap);
                    if (dead) break;
                }
            }
        }
    }else{
        if(score>getHighScore())
            setHightScore(score);

        dead=!(titleScreen->update(enemies,*player,score));
        player->resetPlayerAngle();
    }
}

void Game::render(float deltaTime){
    BeginDrawing();
    ClearBackground(DARKGRAY);

    if(!gameStarted || dead || pause){
        titleScreen->render(dead,score,getHighScore(),pause);
    }else{
        raycast->drawRays3D(*player, myMap);
        myMap.render();
        raycast->drawSprite(*player,enemies);
        player->render(myMap,deltaTime);

        //drawObjectOnMap
        for(GameObject e : enemies){
            DrawRectangle(e.x*Game::miniMapScale+Game::miniMapOffset-Game::miniMapPlayerOffset, e.y*Game::miniMapScale+Game::miniMapOffset-Game::miniMapPlayerOffset, 2, 2, WHITE); 
        }

        //Draw Score
        DrawTextEx(font,TextFormat("SCORE: %d", score), Vector2{static_cast<float>(WINDOW_WIDTH-160),20}, font.baseSize*2.0f, 2, WHITE);

        //Draw Timer
        char const * timerText=TextFormat("TIMER: %.1f", floorTimer);
        Color timerColor=(floorTimer<=1)?RED:WHITE;
        DrawTextEx(font,timerText, Vector2{static_cast<float>(30),static_cast<float>(WINDOW_HEIGHT-50)}, font.baseSize*2.5f, 2, timerColor);

    }
    EndDrawing();
}
