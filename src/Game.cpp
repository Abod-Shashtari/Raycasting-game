#include "Game.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "raylib.h"
#include "Constants.hpp"
#include "RaycastEngine.hpp"
#include "TitleScreen.hpp"
#include <random>
#include <vector>

TitleScreen* titleScreen;
Player* player;
std::vector<GameObject> enemies;
float spawnTimeMax=1.0;
float spawnTimeMin=0.5;
float spwanTimer=0;
std::random_device rd;
std::uniform_int_distribution<int> dist(0,3);
std::uniform_real_distribution<float> distTime(1.5,2);

Font font;
int score=0;

Raycast* raycast;
Map myMap;
bool gameStarted=false;

bool test=false;
void generateEnemies(std::vector<GameObject> &enemies,float deltaTime){
    //alien position
    int randPos=dist(rd);
    //alien spawn
    spwanTimer-=deltaTime;
    if(spwanTimer<=0 && test==false){
        enemies.push_back(*new GameObject(SPWANING_POS[randPos], 1));
        spwanTimer=distTime(rd);
        //test=true;
    }

}
void Game::init(char* title,int width,int height){
    InitWindow(width, height,title);
    titleScreen=new TitleScreen();
    player=new Player(400,200,3);
    raycast=new Raycast();
    font = LoadFont("../assets/alagard.png");
}
void Game::update(float deltaTime){
    if(!gameStarted){
        gameStarted=titleScreen->update();
    }
    if(!dead){
        if (gameStarted){
            player->update(myMap,enemies,score);
            generateEnemies(enemies, deltaTime);
            for(GameObject &e : enemies){
                dead=e.update(*player->getPos(),myMap);
                if (dead) break;
            }
        }
    }else{
        gameStarted=false;
    }
}
void Game::render(float deltaTime){
    BeginDrawing();
    ClearBackground(DARKGRAY);

    if(gameStarted){
        raycast->drawRays3D(*player, myMap);
        myMap.render();
        raycast->drawSprite(*player,enemies);
        player->render(myMap,deltaTime);

        //drawObjectOnMap
        for(GameObject e : enemies){
            DrawRectangle(e.x*Game::miniMapScale+Game::miniMapOffset, e.y*Game::miniMapScale+Game::miniMapOffset, 2, 2, WHITE); 
        }

        //Draw Score
        DrawTextEx(font,TextFormat("SCORE: %d", score), Vector2{static_cast<float>(WINDOW_WIDTH-160),20}, font.baseSize*2.0f, 2, WHITE);

    }else if(!gameStarted || dead){
        titleScreen->render(dead);
    }

    EndDrawing();
}
