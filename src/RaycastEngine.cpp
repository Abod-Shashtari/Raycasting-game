#include "RaycastEngine.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "raylib.h"
#include "../assets/textures.ppm"
#include <cmath>

float distance(float px,float py,float rx,float ry){
    float x=px-rx;
    float y=py-ry;
    return (sqrt((x*x)+(y*y)));
}
float FixAng(float a){
    if(a<0)
        a+=PI*2;
    if(a>2*PI)
        a-=PI*2;
    return a;
}

float Raycast::verticalIntersection(Player& player,Map& map,int mx,int my,int mp,int dof,float rx,float ry,float ra,float xo,float yo, float& vx ,float& vy, int& textureTypeV){
        double nTan= -tan(ra);
        if(ra>P2 && ra<P3){
            rx=(((int)player.getPos()->getPosX()>>6)<<6)-0.0001;
            ry=(player.getPos()->getPosX()-rx)*nTan+player.getPos()->getPosY();
            xo=-64;
            yo=-xo*nTan;
        }//look left
        if(ra<P2 || ra>P3){
            rx=(((int)player.getPos()->getPosX()>>6)<<6)+64;
            ry=(player.getPos()->getPosX()-rx)*nTan+player.getPos()->getPosY();
            xo=64;
            yo=-xo*nTan;
        }//look right
        if(P3==0 || ra==P2){
            ry=player.getPos()->getPosY();
            rx=player.getPos()->getPosX();
            dof=8;
        }
        while(dof<8){
            mx=(int)rx>>6;
            my=(int)ry>>6;
            mp=my*map.getMapX()+mx;
            if((mp>=0 && mp<64) && (map.getMap()[mp]>=1)){
                dof=8;
                vx=rx;
                vy=ry;
                textureTypeV=map.getMap()[mp]-1;
                return distance(player.getPos()->getPosX(),player.getPos()->getPosY(),rx, ry);
            }else{
                rx+=xo;
                ry+=yo;
                dof+=1;
            }
        }
        return INFINITY;
}
float Raycast::horizontalIntersection(Player& player, Map& map, int mx,int my,int mp,int dof,float rx,float ry,float ra,float xo,float yo, float& hx, float& hy, int& textureTypeH){
        //it is cotangent but not cotangent
        double cot = -1 / (tan(ra));
        if(ra>PI){
            ry=(((int)player.getPos()->getPosY()>>6)<<6)-0.0001;
            rx=(player.getPos()->getPosY()-ry)*cot+player.getPos()->getPosX();
            yo=-64;
            xo=-yo*cot;
        }//look up
        if(ra<PI){
            ry=(((int)player.getPos()->getPosY()>>6)<<6)+64;
            rx=(player.getPos()->getPosY()-ry)*cot+player.getPos()->getPosX();
            yo=64;
            xo=-yo*cot;
        }//look down
        if(ra==0 || ra==PI){
            ry=player.getPos()->getPosY();
            rx=player.getPos()->getPosX();
            dof=8;
        }
        while(dof<8){
            mx=(int)rx>>6;
            my=(int)ry>>6;
            mp=my*map.getMapX()+mx;
            if((mp>=0 && mp<64) && (map.getMap()[mp]>=1)){
                dof=8;
                hx=rx;
                hy=ry;
                textureTypeH=map.getMap()[mp]-1;
                return distance(player.getPos()->getPosX(),player.getPos()->getPosY(),rx, ry);
            }else{
                rx+=xo;
                ry+=yo;
                dof+=1;
            }
        }
        return INFINITY;
}
void Raycast::drawRays3D(Player& player, Map& map){
    int r=0, mx=0, my=0, mp=0;
    int dof=0; //depth of view
    float rx=0.0, ry=0.0, ra=0.0; //ray (first) hit position (intercect) and angle
    float xo=0.0, yo=0.0; //intercect position offset
    Color colorV=Color{12, 112, 173,255};
    Color colorH=Color{14, 130, 201,255};

    Color wallColor=BLACK;

    float distH=INFINITY;
    float hx=rx;
    float hy=ry;
    float distV=INFINITY;
    float vx=rx;
    float vy=ry;
    float distR=INFINITY;
    int textureTypeH=0;
    int textureTypeV=0;
    int textureType=0;

    int shade=0;

    //fixAngle-fish
    ra=FixAng(player.getDeltaAngle()->angle-D1*30);

    //run 60 rays
    for (r=0;r<60;r++){
        dof=0;
        distH=horizontalIntersection(player,map, mx, my, mp, dof,  rx,  ry,  ra,  xo,  yo, hx, hy, textureTypeH);
        distV=verticalIntersection(player,map, mx, my, mp, dof,  rx,  ry,  ra,  xo,  yo, vx, vy, textureTypeV);
        if(distH<distV){
            DrawLine(player.getPos()->getPosX()*Game::miniMapScale+Game::miniMapOffset, player.getPos()->getPosY()*Game::miniMapScale+Game::miniMapOffset, hx*Game::miniMapScale+Game::miniMapOffset, hy*Game::miniMapScale+Game::miniMapOffset, GREEN);
            distR=distH;
            rx=hx;
            ry=hy;
            textureType=textureTypeH;
            wallColor=colorH;
            shade=100;
        }else{
            DrawLine(player.getPos()->getPosX()*Game::miniMapScale+Game::miniMapOffset, player.getPos()->getPosY()*Game::miniMapScale+Game::miniMapOffset, vx*Game::miniMapScale+Game::miniMapOffset, vy*Game::miniMapScale+Game::miniMapOffset, GREEN);
            distR=distV;
            rx=vx;
            ry=vy;
            textureType=textureTypeV;
            wallColor=colorV;
            shade=0;
        }
        ra+=D1;
        if(ra<0) ra+=PI*2;
        if(ra>2*PI) ra-=PI*2;
        //-------Draw3D-------
        int windowHeight3D = 320;
        int windowWidth3D = 530;

        //--fixAngle-fish eye--
        float da=FixAng(player.getDeltaAngle()->angle-ra); distR=distR*cos(da);


        int lineHeight=(windowHeight3D*64)/distR;

        float textureYStep=32.0/(float)lineHeight;
        float textureYOffset=0;
        if(lineHeight>windowHeight3D){ 
            textureYOffset=((lineHeight-windowHeight3D)/2.0);
            lineHeight=windowHeight3D;
        }
        int lineOffset=160-(lineHeight>>1);

        float textureY=textureYOffset*textureYStep;
        float textureX;
        if(shade==100){
            //horizontal
            textureX=((int)(rx/2.0))%(32);
            if(player.getDeltaAngle()->angle>0 && player.getDeltaAngle()->angle<PI){
                textureX=31-textureX;
            }

        }else{
            //vertical
            textureX=((int)(ry/2.0))%(32);
            if(player.getDeltaAngle()->angle>P2 && player.getDeltaAngle()->angle<P3){
                textureX=31-textureX;
            }
        }
        textureY+=textureType*32;
        Color color;
        for (int y=0 ;y<lineHeight;y++) {
            float c=All_Textures[((int)(textureY)*32) + (int)textureX];
            if(c==1){
                c=255;
                c-=shade;
            }

            int pixel = ((int)textureY*32+(int)textureX)*3;
            int red = textures[pixel];
            int green = textures[pixel+1];
            int blue = textures[pixel+2];
            Color color = Color{static_cast<unsigned char>(red),static_cast<unsigned char>(green),static_cast<unsigned char>(blue),255};
            //color=Color{static_cast<unsigned char>(c),static_cast<unsigned char>(c),static_cast<unsigned char>(c),255};

            DrawLineEx(Vector2{(float)windowWidth3D+r*8, (float)lineOffset+y-1},Vector2{(float)windowWidth3D+r*8,(float)lineOffset+y} , 8, color);
            textureY+=textureYStep;
        }
        
        //floor and ceiling-------
        for(int y=lineOffset+lineHeight;y<windowHeight3D;y++){
            float dy=y-(windowHeight3D/2.0);
            float deg=ra;
            float raFix=cos(FixAng(player.getDeltaAngle()->angle-ra));//fix
            textureX=player.getPos()->getPosX()/2.0 + cos(deg)*158*32/dy/raFix;
            textureY=player.getPos()->getPosY()/2.0 + sin(deg)*158*32/dy/raFix;
            float c=All_Textures[((int)(textureY)&31)*32 + ((int)(textureX) &31)];
            if(c==1){
                c=255;
            }
            //floor-----
            int floorTextureType=2*32*32;
            int pixel = (((int)(textureY)&31)*32 + ((int)(textureX) &31))*3+floorTextureType*3;
            int red = textures[pixel];
            int green = textures[pixel+1];
            int blue = textures[pixel+2];
            color = Color{static_cast<unsigned char>(red),static_cast<unsigned char>(green),static_cast<unsigned char>(blue),255};

            //color=Color{static_cast<unsigned char>(c),static_cast<unsigned char>(c),static_cast<unsigned char>(c),255};
            DrawLineEx(Vector2{(float)windowWidth3D+(r*8),(float)(y-1)},Vector2{(float)windowWidth3D+r*8,(float)y} , 8, color);

            //ceiling-----
            int ceilingTextureType=3*32*32;
            pixel = (((int)(textureY)&31)*32 + ((int)(textureX) &31))*3+ceilingTextureType*3;
            red = textures[pixel];
            green = textures[pixel+1];
            blue = textures[pixel+2];
            color = Color{static_cast<unsigned char>(red),static_cast<unsigned char>(green),static_cast<unsigned char>(blue),255};

            //c=All_Textures[((int)(textureY)&31)*32 + ((int)(textureX) &31)];
            if(c==1){
                c=255;
            }
            //color=Color{static_cast<unsigned char>(c),static_cast<unsigned char>(c),static_cast<unsigned char>(c),255};
            DrawLineEx(Vector2{(float)windowWidth3D+(r*8),(float)(windowHeight3D-(y-1))},Vector2{(float)windowWidth3D+r*8,(float)(windowHeight3D-y)} , 8, color);
        }
    }

    //draw the texture test
    for (int y=0; y<32; ++y) {
        for (int x=0; x<32; ++x) {

        }
    }
    //
}

