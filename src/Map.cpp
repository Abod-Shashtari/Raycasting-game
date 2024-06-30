#include "Map.hpp"
#include "raylib.h"
#include "Game.hpp"

Color mapColor;

Map::Map(){
    mapX=8;
    mapY=8;
    mapSize=64*Game::miniMapScale;
    map=new int[mapX*mapY];
    int map0[]={
        2,2,2,1,1,1,1,1,
        2,0,1,0,0,0,0,1,
        2,0,5,0,0,0,0,1,
        2,0,1,1,0,0,0,1,
        1,0,5,0,0,0,0,1,
        1,0,1,1,0,1,0,1,
        1,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,
    };
    for(int i=0;i<mapX*mapY;++i){
        map[i]=map0[i];
    }
}
Map::~Map(){
    delete[] map;
}

void Map::render(){
    for (int y=0; y<mapY; ++y) {
        for (int x=0; x<mapX; ++x) {
            if(map[x+(y*mapX)]>=1){
                mapColor=BLUE;
            }else{
                mapColor=BLACK;
            }
            DrawRectangle(x*mapSize+1+Game::miniMapOffset, y*mapSize+1+Game::miniMapOffset, mapSize-1, mapSize-1, mapColor);
        }
    
    }
}

int* Map::getMap(){
    return map;
}
int Map::getMapX(){
    return mapX;
}
int Map::getMapY(){
    return mapY;
}

