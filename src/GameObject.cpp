#include "GameObject.hpp"
#include "Constants.hpp"
#include "Vector2D.hpp"
#include <cmath>

GameObject::GameObject(Vector2D pos,float speed){
    this->x=pos.getPosX();
    this->y=pos.getPosY();
    this->speed=speed;
}
int offset=10;
bool GameObject::update(Vector2D playerPos,Map &map){
    if(playerPos.getPosX()>x){
        int newX,newY;
        newX=(x+offset)/WALL_SIZE;
        newY=y/WALL_SIZE;

        int mapPos=(int)(newY*map.getMapX()+newX);
        if(mapPos>0 && mapPos<map.getMapSize() && (map.getMap()[mapPos]==0))
            x+=speed;
    }
    if(playerPos.getPosX()<x){
        int newX,newY;
        newX=(x-offset)/WALL_SIZE;
        newY=y/WALL_SIZE;

        int mapPos=(int)(newY*map.getMapX()+newX);
        if(mapPos>0 && mapPos<map.getMapSize() && (map.getMap()[mapPos]==0))
            x-=speed;
    }
    if(playerPos.getPosY()>y){
        int newX,newY;
        newY=(y+offset)/WALL_SIZE;
        newX=x/WALL_SIZE;

        int mapPos=(int)(newY*map.getMapX()+newX);
        if(mapPos>0 && mapPos<map.getMapSize() && (map.getMap()[mapPos]==0))
            y+=speed;
    }
    if(playerPos.getPosY()<y){
        int newX,newY;
        newY=(y-offset)/WALL_SIZE;
        newX=x/WALL_SIZE;
        
        int mapPos=(int)(newY*map.getMapX()+newX);
        if(mapPos>0 && mapPos<map.getMapSize() && (map.getMap()[mapPos]==0))
            y-=speed;
    }
    float distance = sqrt(pow(playerPos.getPosX()-x,2)+pow(playerPos.getPosY()-y,2));
    return (distance<30);
}
void GameObject::render(){
}
GameObject::~GameObject() {
}
