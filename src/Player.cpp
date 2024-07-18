#include "Player.hpp"
#include "Game.hpp"
#include "raylib.h"
#include <cmath>
#include <iostream>
#include <vector>
#include "../assets/playerText.ppm"
#include "../assets/fireText.ppm"

int dir[4]={0};
int doorDir[4]={0};
float fireSpriteTime=0;
float const fireSpriteDuration=0.1;
Player::Player(float x, float y,int speed){
    this->speed=speed;
    pos=new Vector2D(100,200);
    float pa=PI/2.0;
    float pdx=cos(pa)*speed;
    float pdy=sin(pa)*speed;
    deltaAngle= new DeltaAngle(pdx,pdy,pa);
    InitAudioDevice();
    gunShotWav=LoadSound("../assets/gunshot.wav");
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
void Player::update(Map& map,std::vector<GameObject> &enemies,int& score){
    if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)){
        dir[0]={0};dir[1]={0};dir[2]={0};dir[3]={0};
        collide(*pos, *deltaAngle, map, 10, dir);

        //no collision with the wall
        if(dir[0]==0)
            pos->setPosX(pos->getPosX()+deltaAngle->dx);
        if(dir[1]==0)
            pos->setPosY(pos->getPosY()+deltaAngle->dy);

    }
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        deltaAngle->angle-=0.06;
        if(deltaAngle->angle<0) deltaAngle->angle+=2*PI;
        deltaAngle->dx=cos(deltaAngle->angle)*speed;
        deltaAngle->dy=sin(deltaAngle->angle)*speed;
    }
    if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){
        dir[0]={0};dir[1]={0};dir[2]={0};dir[3]={0};
        collide(*pos, *deltaAngle, map, 10, dir);

        //no collision with the wall
        if(dir[2]==0)
            pos->setPosX(pos->getPosX()-deltaAngle->dx);
        if(dir[3]==0)
            pos->setPosY(pos->getPosY()-deltaAngle->dy);
    }
    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
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
    //shoot
    if(IsKeyPressed(KEY_LEFT_CONTROL)){
        PlaySound(gunShotWav);
        fireSpriteTime=fireSpriteDuration;
        float playerDeg=(this->getDeltaAngle()->angle)*RAD2DEG;
        int index=0;
        for(GameObject e : enemies){
            //enemy angle
            float vx=e.x-this->getPos()->getPosX();
            float vy=e.y-this->getPos()->getPosY();
            float enemyAngle=atan2(vy,vx)*RAD2DEG;
            if(enemyAngle>360)enemyAngle-=360;
            if(enemyAngle<0)enemyAngle+=360;

            if(abs(playerDeg-enemyAngle)<=12 && e.seen) {
                score++;
                enemies.erase(enemies.begin()+index);
                break;
            }
            ++index;
        }
    }
}
Vector2D* Player::getPos(){
    return pos;
}
DeltaAngle* Player::getDeltaAngle(){
    return deltaAngle;
}
void drawHand(){
    int offsetX=540;
    int offsetY=330;
    int reslution=2;
    for(int i=0;i<325;i+=reslution){
        for(int j=0;j<366;j+=reslution){
                int pixel = (i*366+j)*3;
                int red = playerText[pixel];
                int green = playerText[pixel + 1];
                int blue = playerText[pixel + 2];

                int alpha =(red==255 && green ==255 && blue==255)?0:255;
                Color color={static_cast<unsigned char>(red),static_cast<unsigned char>(green),static_cast<unsigned char>(blue),static_cast<unsigned char>(alpha)};
                Vector2 v1={static_cast<float>(j+offsetX),static_cast<float>(i+offsetY)};
                Vector2 v2={static_cast<float>(j+offsetX),static_cast<float>(i+offsetY+reslution)};
                DrawLineEx(v1, v2, reslution, color);
        }
    }
}
void drawFire(){
    int offsetX=590;
    int offsetY=370;
    int reslution=2;
    int scale=4;
    int scaledSize=32*scale;
    for(int i=0;i<scaledSize;i+=reslution){
        for(int j=0;j<scaledSize;j+=reslution){
                int pixel = ((i/scale)*32+(j/scale))*3;
                int red = fireText[pixel];
                int green = fireText[pixel + 1];
                int blue = fireText[pixel + 2];

                int alpha =(red==255 && green ==0 && blue==255)?0:255;
                Color color={static_cast<unsigned char>(red),static_cast<unsigned char>(green),static_cast<unsigned char>(blue),static_cast<unsigned char>(alpha)};
                Vector2 v1={static_cast<float>(j+offsetX-scaledSize/2.0),static_cast<float>(i+offsetY-scaledSize/2.0)};
                Vector2 v2={static_cast<float>(j+offsetX-scaledSize/2.0),static_cast<float>(i+offsetY+reslution-scaledSize/2.0)};
                DrawLineEx(v1, v2, reslution, color);
        }
    }
}
void Player::render(Map& map,float deltaTime){
    int width=4;
    int height=4;
    float drawPosX=pos->getPosX()*Game::miniMapScale+Game::miniMapOffset-Game::miniMapPlayerOffset;
    float drawPosY=pos->getPosY()*Game::miniMapScale+Game::miniMapOffset-Game::miniMapPlayerOffset;
    DrawLine(pos->getPosX()*Game::miniMapScale+Game::miniMapOffset, pos->getPosY()*Game::miniMapScale+Game::miniMapOffset, (pos->getPosX()+deltaAngle->dx)*Game::miniMapScale+Game::miniMapOffset,(pos->getPosY()+deltaAngle->dy)*Game::miniMapScale+Game::miniMapOffset, RED);
    DrawRectangle(drawPosX, drawPosY, width, height, RED); 
    fireSpriteTime=(fireSpriteTime>0)?(fireSpriteTime-deltaTime):(0);
    if(fireSpriteTime>0) drawFire();
    drawHand();
}

