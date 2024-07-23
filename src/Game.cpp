#include "Game.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Vector2D.hpp"
#include "raylib.h"
#include "Constants.hpp"
#include "RaycastEngine.hpp"
#include "TitleScreen.hpp"
#include <iostream>
#include <random>
#include <vector>

TitleScreen* titleScreen;
Player* player;
std::vector<GameObject> enemies;
float const spawnTimeMax=1.8;
float const spawnTimeMin=1.7;
float spwanTimer=0;
float const floorTime=3.0;
float const coolFloorTime=5.0;
float coolTimer=coolFloorTime;
float floorTimer=floorTime;
int playerOldMapPosX;
int playerOldMapPosY;
std::vector<Vector2D> hotFloorPos;

std::random_device rd;
std::uniform_int_distribution<int> dist(0,3);
std::uniform_real_distribution<float> distTime(spawnTimeMin,spawnTimeMax);

Font font;
int score=0;

Raycast* raycast;
Map myMap;
bool gameStarted=false;

bool test=false;
//int count=0;
void generateEnemies(std::vector<GameObject> &enemies,float deltaTime){
    //alien position
    int randPos=dist(rd);
    //alien spawn
    spwanTimer-=deltaTime;
    if(spwanTimer<=0 && test==false){
        enemies.push_back(*new GameObject(SPWANING_POS[randPos], 1.5));
        spwanTimer=distTime(rd);
            //count++;
        //if(count>2){
            //test=true;
    }

}
void hotFloor(float deltaTime){
    int mapPosX=player->getPos()->getPosX()/64.0;
    int mapPosY=player->getPos()->getPosY()/64.0;
    if(mapPosX==playerOldMapPosX && mapPosY==playerOldMapPosY){
        floorTimer-=deltaTime;
        if(floorTimer<=0){
            myMap.getMapF()[mapPosY*myMap.getMapX()+mapPosX]=5;
            hotFloorPos.push_back(Vector2D(mapPosX,mapPosY));
            floorTimer=floorTime;
        }
    }else{
        floorTimer=floorTime;
        playerOldMapPosX=player->getPos()->getPosX()/64.0;
        playerOldMapPosY=player->getPos()->getPosY()/64.0;
    }
}
void coolFloor(float deltaTime){
    if(!hotFloorPos.empty()){
        coolTimer-=deltaTime;
        if(coolTimer<=0){
            int mapPosX=hotFloorPos.begin()->getPosX();
            int mapPosY=hotFloorPos.begin()->getPosY();
            myMap.getMapF()[mapPosY*myMap.getMapX()+mapPosX]=2;
            hotFloorPos.erase(hotFloorPos.begin());
            coolTimer=coolFloorTime;
        }
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

            player->update(myMap,enemies,score);
            hotFloor(deltaTime);
            coolFloor(deltaTime);
            generateEnemies(enemies, deltaTime);

            for(GameObject &e : enemies){
                dead=e.update(*player->getPos(),myMap);
                if (dead) break;
            }
        }
    }else{
        dead=!(titleScreen->update(enemies,*player,score));
    }
}
void Game::render(float deltaTime){
    BeginDrawing();
    ClearBackground(DARKGRAY);

    if(!gameStarted || dead){
        titleScreen->render(dead,score);
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
        char const * timerText=TextFormat("TIMER: %.1f", floorTimer);
        Color timerColor=(floorTimer<=1)?RED:WHITE;
        DrawTextEx(font,timerText, Vector2{static_cast<float>(30),static_cast<float>(WINDOW_HEIGHT-50)}, font.baseSize*2.5f, 2, timerColor);

    }
    EndDrawing();
}
