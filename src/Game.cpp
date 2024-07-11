#include "Game.hpp"
#include "Player.hpp"
#include "raylib.h"
#include "RaycastEngine.hpp"

Player* player=new Player(400,200,3);
std::vector<GameObject> enemies;
GameObject* s1 = new GameObject(350,210);
/*
GameObject* s2 = new GameObject(353,240);
GameObject* s3 = new GameObject(310,210);
GameObject* s4 = new GameObject(340,90);
*/

Raycast* raycast=new Raycast();
Map myMap;
void Game::init(char* title,int width,int height){
    InitWindow(width, height,title);
    enemies.push_back(*s1);
    /*
    enemies.push_back(*s2);
    enemies.push_back(*s3);
    enemies.push_back(*s4);
    */
}
void Game::update(){
    player->update(myMap,enemies);
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
        DrawRectangle(e.x*Game::miniMapScale+Game::miniMapOffset, e.y*Game::miniMapScale+Game::miniMapOffset, 4, 4, WHITE); 
    }

    EndDrawing();
}
