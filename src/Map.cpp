#include "Map.hpp"
#include "raylib.h"
#include "Game.hpp"

Color mapColor;

Map::Map(){
    mapX=11;
    mapY=11;
    mapSize=64*Game::miniMapScale; //64 is the wall size (block size)
    map=new int[mapX*mapY];
    mapF=new int[mapX*mapY];
    int map0[]={
        0,0,0,0,2,2,2,0,0,0,0,
        0,0,0,0,2,0,2,0,0,0,0,
        0,0,1,1,1,0,1,1,1,0,0,
        0,0,1,0,0,0,0,0,1,0,0,
        2,2,1,0,0,0,0,0,1,2,2,
        2,0,0,0,0,0,0,0,0,0,2,
        2,2,1,0,0,0,0,0,1,2,2,
        0,0,1,0,0,0,0,0,1,0,0,
        0,0,1,1,1,0,1,1,1,0,0,
        0,0,0,0,2,0,2,0,0,0,0,
        0,0,0,0,2,2,2,0,0,0,0,
    };
    int map0F[]={
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,
    };
    for(int i=0;i<getMapSize();++i){
        map[i]=map0[i];
    }
    for(int i=0;i<getMapSize();++i){
        mapF[i]=map0F[i];
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
                DrawRectangle(x*mapSize+1+Game::miniMapOffset, y*mapSize+1+Game::miniMapOffset, mapSize-1, mapSize-1, mapColor);
            }else{
                mapColor=BLACK;
                DrawRectangleLines(x*mapSize+1+Game::miniMapOffset, y*mapSize+1+Game::miniMapOffset, mapSize-1, mapSize-1, mapColor);
            }
        }
    
    }
}

int* Map::getMap(){
    return map;
}
int* Map::getMapF(){
    return mapF;
}
int Map::getMapX(){
    return mapX;
}
int Map::getMapY(){
    return mapY;
}
int Map::getMapSize(){
    return 121;
}

