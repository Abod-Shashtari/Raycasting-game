#include "Player.hpp"
#include "raylib.h"
#include <cmath>

int dir[4]={0};
int doorDir[4]={0};
float degToRadP(float a) { return a*PI/180.0;}
float FixAngP(float a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}

Player::Player(float x, float y,int speed){
    this->speed=speed;
    pos=new Vector2D(x,y);
    pdx=cos(degToRadP(pa));
    pdy=-sin(degToRadP(pa));
    deltaAngle= new DeltaAngle(pdx,pdy,pa);
}
Player::~Player(){
}
void collide(Vector2D& pos, DeltaAngle& deltaAngle, Map& map, int offset, int dir[]){
    int xOff,yOff;
    int nxOff,nyOff;
    int offsetX=offset;
    int offsetY=offset;

    if(deltaAngle.dx<0)
        offsetX=-offsetX;
    if(deltaAngle.dy<0)
        offsetY=-offsetY;

    xOff=((int)pos.getPosX()+offsetX)>>6;
    yOff=((int)pos.getPosY()+offsetY)>>6;

    nxOff=((int)pos.getPosX()-offsetX)>>6;
    nyOff=((int)pos.getPosY()-offsetY)>>6;

    int pmx=(int)pos.getPosX()>>6;
    int pmy=(int)pos.getPosY()>>6;

    int xAxisWall=pmy*map.getMapX()+xOff;
    int yAxisWall=yOff*map.getMapX()+pmx;
    int nxAxisWall=pmy*map.getMapX()+nxOff;
    int nyAxisWall=nyOff*map.getMapX()+pmx;
    
    int walls[]={xAxisWall,yAxisWall,nxAxisWall,nyAxisWall};
    for(int i=0;i<4;++i){
        if((map.getMap()[walls[i]]>=1 && (offset<=10))){
            dir[i]=1;
        }
        if((map.getMap()[walls[i]]==4 && (offset>10))){
            dir[i]=walls[i];
        }
    }

}
void Player::update(Map& map){
    if(IsKeyDown(KEY_W)){
        dir[0]={0};dir[1]={0};dir[2]={0};dir[3]={0};
        collide(*pos, *deltaAngle, map, 10, dir);
        //Collide with Wall
        if(dir[0]==0)
            pos->setPosX(pos->getPosX()+deltaAngle->dx);
        if(dir[1]==0)
            pos->setPosY(pos->getPosY()+deltaAngle->dy);

    }
    if(IsKeyDown(KEY_A)){
        deltaAngle->angle+=2.5;
        FixAngP(deltaAngle->angle);
        deltaAngle->dx=cos(degToRadP(deltaAngle->angle))*speed;
        deltaAngle->dy=-sin(degToRadP(deltaAngle->angle))*speed;
    }
    if(IsKeyDown(KEY_S)){
        dir[0]={0};dir[1]={0};dir[2]={0};dir[3]={0};
        collide(*pos, *deltaAngle, map, 10, dir);
        if(dir[2]==0)
            pos->setPosX(pos->getPosX()-deltaAngle->dx);
        if(dir[3]==0)
            pos->setPosY(pos->getPosY()-deltaAngle->dy);
    }
    if(IsKeyDown(KEY_D)){
        deltaAngle->angle-=2.5;
        FixAngP(deltaAngle->angle);
        deltaAngle->dx=cos(degToRadP(deltaAngle->angle))*speed;
        deltaAngle->dy=-sin(degToRadP(deltaAngle->angle))*speed;
    }
    if(IsKeyPressed(KEY_E)){
        doorDir[0]={-1};doorDir[1]={-1};doorDir[2]={-1};doorDir[3]={-1};
        collide(*pos, *deltaAngle, map, 25, doorDir);

        for(int i=0;i<4;++i){
            if(doorDir[i]!=-1)
                map.getMap()[doorDir[i]]=0;
        }
    }
}
Vector2D* Player::getPos(){
    return pos;
}
DeltaAngle* Player::getDeltaAngle(){
    return deltaAngle;
}
void Player::render(Map& map){
    int width=5;
    int height=5;
    DrawLine(pos->getPosX(), pos->getPosY(), pos->getPosX()+deltaAngle->dx,pos->getPosY()+deltaAngle->dy, RED);
    DrawRectangle(pos->getPosX(), pos->getPosY(), width, height, RED); 
}

