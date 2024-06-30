#include "Player.hpp"
#include "Game.hpp"
#include "raylib.h"
#include <cmath>

int dir[4]={0};
int doorDir[4]={0};
Player::Player(float x, float y,int speed){
    this->speed=speed;
    pos=new Vector2D(x,y);
    pdx=cos(pa)*speed;
    pdy=sin(pa)*speed;
    deltaAngle= new DeltaAngle(pdx,pdy,pa);
}
Player::~Player(){
}
void collide(Vector2D& pos, DeltaAngle& deltaAngle, Map& map, int offset, int dir[]){
    int xOff,yOff; //collision from the front
    int nxOff,nyOff; //collision from the back
    int offsetX=offset;
    int offsetY=offset;

    if(deltaAngle.dx<0)
        offsetX=-offsetX;
    if(deltaAngle.dy<0)
        offsetY=-offsetY;

    //divide by 64 , cell size = 64player.
    xOff=((int)pos.getPosX()+offsetX)>>6;
    yOff=((int)pos.getPosY()+offsetY)>>6;

    nxOff=((int)pos.getPosX()-offsetX)>>6;
    nyOff=((int)pos.getPosY()-offsetY)>>6;

    int pmx=(int)pos.getPosX()>>6;
    int pmy=(int)pos.getPosY()>>6;

    //position of right and bottom of the player 
    int xAxisWall=pmy*map.getMapX()+xOff;
    int yAxisWall=yOff*map.getMapX()+pmx;

    //position of left and top of the player 
    int nxAxisWall=pmy*map.getMapX()+nxOff;
    int nyAxisWall=nyOff*map.getMapX()+pmx;
    
    int walls[]={xAxisWall,yAxisWall,nxAxisWall,nyAxisWall};
    for(int i=0;i<4;++i){
        if((map.getMap()[walls[i]]>=1 && (offset<=10))){
            dir[i]=1;
        }
        //check if there is door (4 means door)
        if((map.getMap()[walls[i]]==5 && (offset>10))){
            dir[i]=walls[i];
        }
    }

}
void Player::update(Map& map){
    if(IsKeyDown(KEY_W)){
        dir[0]={0};dir[1]={0};dir[2]={0};dir[3]={0};
        collide(*pos, *deltaAngle, map, 10, dir);

        //no collision with the wall
        if(dir[0]==0)
            pos->setPosX(pos->getPosX()+deltaAngle->dx);
        if(dir[1]==0)
            pos->setPosY(pos->getPosY()+deltaAngle->dy);

    }
    if(IsKeyDown(KEY_A)){
        deltaAngle->angle-=0.06;
        if(deltaAngle->angle<0) deltaAngle->angle+=2*PI;
        deltaAngle->dx=cos(deltaAngle->angle)*speed;
        deltaAngle->dy=sin(deltaAngle->angle)*speed;
    }
    if(IsKeyDown(KEY_S)){
        dir[0]={0};dir[1]={0};dir[2]={0};dir[3]={0};
        collide(*pos, *deltaAngle, map, 10, dir);

        //no collision with the wall
        if(dir[2]==0)
            pos->setPosX(pos->getPosX()-deltaAngle->dx);
        if(dir[3]==0)
            pos->setPosY(pos->getPosY()-deltaAngle->dy);
    }
    if(IsKeyDown(KEY_D)){
        deltaAngle->angle+=0.06;
        if(deltaAngle->angle>2*PI) deltaAngle->angle-=2*PI;
        deltaAngle->dx=cos(deltaAngle->angle)*speed;
        deltaAngle->dy=sin(deltaAngle->angle)*speed;
    }
    if(IsKeyPressed(KEY_E)){
        doorDir[0]={-1};doorDir[1]={-1};doorDir[2]={-1};doorDir[3]={-1};
        collide(*pos, *deltaAngle, map, 25, doorDir);

        for(int i=0;i<4;++i){
            if(doorDir[i]!=-1)
                //open the door
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
    int width=4;
    int height=4;
    DrawLine(pos->getPosX()*Game::miniMapScale+Game::miniMapOffset, pos->getPosY()*Game::miniMapScale+Game::miniMapOffset, (pos->getPosX()+deltaAngle->dx)*Game::miniMapScale+Game::miniMapOffset,(pos->getPosY()+deltaAngle->dy)*Game::miniMapScale+Game::miniMapOffset, RED);
    DrawRectangle(pos->getPosX()*Game::miniMapScale+Game::miniMapOffset, pos->getPosY()*Game::miniMapScale+Game::miniMapOffset, width, height, RED); 
}

