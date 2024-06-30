#include "RaycastEngine.hpp"
#include "Map.hpp"
#include "raylib.h"
#include <cmath>

float distance(float px,float py,float rx,float ry){
    float x=px-rx;
    float y=py-ry;
    return (sqrt((x*x)+(y*y)));
}
float degToRad(float a) { return a*PI/180.0;}
float FixAng(float a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}

float Raycast::verticalIntersection(Player& player,Map& map,int mx,int my,int mp,int dof,float rx,float ry,float ra,float xo,float yo, float& vx ,float& vy, int& textureTypeV){
        float Tan=tan(degToRad(ra));
        if(cos(degToRad(ra))> 0.001){
            rx=(((int)player.getPos()->getPosX()>>6)<<6)+64;
            ry=(player.getPos()->getPosX()-rx)*Tan+player.getPos()->getPosY();
            xo= 64;
            yo=-xo*Tan;
        }//looking left
        else if(cos(degToRad(ra))<-0.001){
            rx=(((int)player.getPos()->getPosX()>>6)<<6) -0.0001;
            ry=(player.getPos()->getPosX()-rx)*Tan+player.getPos()->getPosY();
            xo=-64;
            yo=-xo*Tan;
        }//looking right
        else {
            rx=player.getPos()->getPosX();
            ry=player.getPos()->getPosY();
            dof=8;
        }//looking up or down. no hit
        while(dof<8){
            mx=(int)rx>>6;
            my=(int)ry>>6;
            mp=my*map.getMapX()+mx;
            if((mp>=0 && mp<64) && (map.getMap()[mp]>=1)){
                dof=8;
                vx=rx;
                vy=ry;
                textureTypeV=map.getMap()[mp]-1;
                return cos(degToRad(ra))*(rx-player.getPos()->getPosX()) - sin(degToRad(ra))*(ry-player.getPos()->getPosY());//hit
            }else{
                rx+=xo;
                ry+=yo;
                dof+=1;
            }
        }
        return INFINITY;
}
float Raycast::horizontalIntersection(Player& player, Map& map, int mx,int my,int mp,int dof,float rx,float ry,float ra,float xo,float yo, float& hx, float& hy, int& textureTypeH){
        float Tan=tan(degToRad(ra));
        Tan=1.0/Tan; 
        if(sin(degToRad(ra))> 0.001){
            ry=(((int)player.getPos()->getPosY()>>6)<<6) -0.0001;
            rx=(player.getPos()->getPosY()-ry)*Tan+player.getPos()->getPosX();
            yo=-64; xo=-yo*Tan;
        }//looking up 
        else if(sin(degToRad(ra))<-0.001){
            ry=(((int)player.getPos()->getPosY()>>6)<<6)+64;
            rx=(player.getPos()->getPosY()-ry)*Tan+player.getPos()->getPosX();
            yo= 64;
            xo=-yo*Tan;
        }//looking down
        else{
            rx=player.getPos()->getPosX();
            ry=player.getPos()->getPosY();
            dof=8;
        }
        while(dof<8){
            mx=(int)rx>>6;
            my=(int)ry>>6;
            mp=my*map.getMapX()+mx;
            if(mp>0 && mp<map.getMapX()*map.getMapY() && map.getMap()[mp]>0){
                textureTypeH=map.getMap()[mp]-1;
                dof=8;
                hx=rx;
                hy=ry;
                dof=8;
                return cos(degToRad(ra))*(rx-player.getPos()->getPosX())-sin(degToRad(ra))*(ry-player.getPos()->getPosY());
            }//hit         
            else{ rx+=xo; ry+=yo; dof+=1;} //check next horizontal
        }
        return INFINITY;
}
void Raycast::drawRays3D(Player& player, Map& map){
    int r=0, mx=0, my=0, mp=0, dof=0;
    float rx=0.0, ry=0.0, ra=0.0, xo=0.0, yo=0.0;
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
    ra=FixAng(player.getDeltaAngle()->angle+30);

     for(r=0;r<60;r++){
        dof=0;
        distH=horizontalIntersection(player,map, mx, my, mp, dof,  rx,  ry,  ra,  xo,  yo, hx, hy, textureTypeH);
        distV=verticalIntersection(player,map, mx, my, mp, dof,  rx,  ry,  ra,  xo,  yo, vx, vy, textureTypeV);
        if(distH<distV){
            DrawLine(player.getPos()->getPosX(), player.getPos()->getPosY(), hx, hy, GREEN);
            distR=distH;
            rx=hx;
            ry=hy;
            textureType=textureTypeH;
            wallColor=colorH;
            shade=100;
        }else{
            DrawLine(player.getPos()->getPosX(), player.getPos()->getPosY(), vx, vy, GREEN);
            distR=distV;
            rx=vx;
            ry=vy;
            textureType=textureTypeV;
            wallColor=colorV;
            shade=0;
        }
        int ca=FixAng(player.getDeltaAngle()->angle-ra);
        distR=distR*cos(degToRad(ca));
        //-------Draw3D-------
        int windowHeight3D = 320;
        int windowWidth3D = 530;

        int lineH = (64*320)/(distR); 
        float ty_step=32.0/(float)lineH; 
        float ty_off=0; 
        if(lineH>320){ ty_off=(lineH-320)/2.0; lineH=320;}                            //line height and limit
        int lineOff = 160 - (lineH>>1);                                               //line offset
          
        int y;
        int hmt=0;
        float ty=ty_off*ty_step+hmt*32;
        float tx;

        if(shade==100){
            //horizontal
            tx=((int)(rx/2.0))%(32);
            if(ra>180){
                tx=31-tx;
            }
        }else{
            //vertical
            tx=((int)(ry/2.0))%(32);
            if(ra>90 && ra<270){
                tx=31-tx;
            }
        }
        ty+=textureType*32;
        Color color;
        for (int y=0 ;y<lineH;y++) {
            float c=All_Textures[((int)(ty)*32) + (int)tx];
            if(c==1){
                c=255;
                c-=shade;
            }
            color=Color{static_cast<unsigned char>(c),static_cast<unsigned char>(c),static_cast<unsigned char>(c),255};
            DrawLineEx(Vector2{(float)windowWidth3D+r*8, (float)lineOff+y-1},Vector2{(float)windowWidth3D+r*8,(float)lineOff+y} , 8, color);
            ty+=ty_step;
        }
        //DrawLineEx(Vector2{(float)windowWidth3D+r*8, lineOffset},Vector2{(float)windowWidth3D+r*8,lineHeight+lineOffset} , 8, wallColor);
        
        for(y=lineOff+lineH;y<320;y++)
        {
          float dy=y-(320/2.0), deg=degToRad(ra), raFix=cos(degToRad(FixAng(player.getDeltaAngle()->angle-ra)));
          tx=player.getPos()->getPosX()/2 + cos(deg)*158*32/dy/raFix;
          ty=player.getPos()->getPosY()/2 - sin(deg)*158*32/dy/raFix;
          float c=All_Textures[((int)(ty)&31)*32 + ((int)(tx) &31)];
          if(c==1){
            c=255;
          }
            color=Color{static_cast<unsigned char>(c),static_cast<unsigned char>(c),static_cast<unsigned char>(c),255};
            DrawLineEx(Vector2{(float)windowWidth3D+(r*8),(float)(y-1)},Vector2{(float)windowWidth3D+r*8,(float)y} , 8, color);

         }
         
        ra=FixAng(ra-1);                                                               //go to next ray, 60 total
    }
}

