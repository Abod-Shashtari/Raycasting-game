#include "Game.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "raylib.h"
#include "Constants.hpp"
#include "RaycastEngine.hpp"
#include <random>
#include <vector>

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


Raycast* raycast=new Raycast();
Map myMap;

void generateEnemies(std::vector<GameObject> &enemies,float deltaTime){
    //alien position
    int randPos=dist(rd);
    //alien spawn
    spwanTimer-=deltaTime;
    if(spwanTimer<=0){
        enemies.push_back(*new GameObject(SPWANING_POS[randPos], 1));
        spwanTimer=distTime(rd);
    }

}
void Game::init(char* title,int width,int height){
    InitWindow(width, height,title);
    player=new Player(400,200,3);
    font = LoadFont("../assets/alagard.png");
}
void Game::update(float deltaTime){
    player->update(myMap,enemies,score);
    generateEnemies(enemies, deltaTime);
    for(GameObject &e : enemies){
        e.update(*player->getPos(),myMap);
    }
}
void Game::render(){
    BeginDrawing();
    ClearBackground(DARKGRAY);
    raycast->drawRays3D(*player, myMap);
    myMap.render();
    raycast->drawSprite(*player,enemies);
    player->render(myMap);

    //drawObjectOnMap
    for(GameObject e : enemies){
        DrawRectangle(e.x*Game::miniMapScale+Game::miniMapOffset, e.y*Game::miniMapScale+Game::miniMapOffset, 2, 2, WHITE); 
    }

    //Draw Score
    DrawTextEx(font,TextFormat("SCORE: %d", score), Vector2{static_cast<float>(WINDOW_WIDTH-160),20}, font.baseSize*2.0f, 2, WHITE);

    EndDrawing();
}
