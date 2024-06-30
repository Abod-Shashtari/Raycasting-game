#include "Game.hpp"
#include "raylib.h"
#include "RaycastEngine.hpp"
#include "Player.hpp"

Player* player=new Player(400,200,3);
//Player* player=new Player(25,25,3);
Raycast* raycast=new Raycast();
Map myMap;
void Game::init(char* title,int width,int height){
    InitWindow(width, height,title);
}
void Game::update(){
    player->update(myMap);
}
void Game::render(){
    BeginDrawing();
    ClearBackground(DARKGRAY);
    myMap.render();
    raycast->drawRays3D(*player, myMap);
    player->render(myMap);
    EndDrawing();
}
