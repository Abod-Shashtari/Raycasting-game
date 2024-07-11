#include "GameObject.hpp"
#include "Vector2D.hpp"

GameObject::GameObject(float x, float y){
    this->x=x;
    this->y=y;
}
int offset=10;
void GameObject::update(Vector2D playerPos,Map &map){
    if(playerPos.getPosX()>x){
        int newX,newY;
        newX=(x+offset)/map.getMapSize();
        newY=y/map.getMapSize();

        int mapPos=(int)(newY*map.getMapX()+newX);
        if(mapPos>0 && mapPos<map.getMapSize() && (map.getMap()[mapPos]==0))
            x+=speed;
    }
    if(playerPos.getPosX()<x){
        int newX,newY;
        newX=(x-offset)/map.getMapSize();
        newY=y/map.getMapSize();

        int mapPos=(int)(newY*map.getMapX()+newX);
        if(mapPos>0 && mapPos<map.getMapSize() && (map.getMap()[mapPos]==0))
            x-=speed;
    }
    if(playerPos.getPosY()>y){
        int newX,newY;
        newY=(y+offset)/map.getMapSize();
        newX=x/map.getMapSize();

        int mapPos=(int)(newY*map.getMapX()+newX);
        if(mapPos>0 && mapPos<map.getMapSize() && (map.getMap()[mapPos]==0))
            y+=speed;
    }
    if(playerPos.getPosY()<y){
        int newX,newY;
        newY=(y-offset)/map.getMapSize();
        newX=x/map.getMapSize();
        
        int mapPos=(int)(newY*map.getMapX()+newX);
        if(mapPos>0 && mapPos<map.getMapSize() && (map.getMap()[mapPos]==0))
            y-=speed;
    }
}
void GameObject::render(){
}
GameObject::~GameObject() {
}
