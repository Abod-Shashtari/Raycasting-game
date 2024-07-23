#include "RaycastEngine.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "raylib.h"

#include "../assets/textures64.ppm"
#include "../assets/textures32.ppm"
#include "../assets/alien.ppm"
#include "../assets/wall64.ppm"
#include "Constants.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
//--------


float distance(float px,float py,float rx,float ry){
    float x=px-rx;
    float y=py-ry;
    return (sqrt((x*x)+(y*y)));
}
//fixing the fish eye effect
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
        if(ra==P3 || ra==P2){
            ry=player.getPos()->getPosY();
            rx=player.getPos()->getPosX();
            dof=10;
        }
        while(dof<10){
            mx=(int)rx>>6;
            my=(int)ry>>6;
            mp=my*map.getMapX()+mx;
            if((mp>=0 && mp<map.getMapSize()) && (map.getMap()[mp]>=1)){
                dof=10;
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
        if(ra==0 || ra==(2*PI) || ra==PI){
            ry=player.getPos()->getPosY();
            rx=player.getPos()->getPosX();
            dof=10;
        }
        while(dof<10){
            mx=(int)rx>>6;
            my=(int)ry>>6;
            mp=my*map.getMapX()+mx;
            if((mp>=0 && mp<map.getMapSize()) && (map.getMap()[mp]>=1)){
                dof=10;
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
void printDist(std::vector<GameObject> eneimes){
    std::cout<<"====="<<std::endl;
    int i=0;
    for(GameObject &s : eneimes){
        ++i;
        std::cout<<i<<": "<<s.distnaceFromPlayer<<std::endl;
    }
    std::cout<<"====="<<std::endl;

}
void Raycast::drawSprite(Player& player, std::vector<GameObject> &eneimes){
    //sort eneimes by distance from player
    for(GameObject &s : eneimes){
        float hx=s.x-player.getPos()->getPosX();
        float hy=s.y-player.getPos()->getPosY();
        s.distnaceFromPlayer=distance(0, 0, hx, hy);
    }
    //bubble sort
    for(int i=0;i<eneimes.size();++i){
        for(int j=0;j<eneimes.size()-1;++j){
            if(eneimes[j+1].distnaceFromPlayer>eneimes[j].distnaceFromPlayer){
                std::iter_swap(&eneimes[j], &eneimes[j+1]);
            }
        }
    }

    for(GameObject &s : eneimes){
        float hx=s.x-player.getPos()->getPosX();
        float hy=s.y-player.getPos()->getPosY();

        float p = atan2(hy,hx)*RAD2DEG;
        float pa = player.getDeltaAngle()->angle*RAD2DEG;

        float px=player.getPos()->getPosX();
        float py=player.getPos()->getPosY();

        float distanceToSprite=distance(0, 0, hx, hy);

        if(p>360){
            p-=360;
        }
        if (p<0) {
            p+=360;
        }

        float q = (HALF_FOV)+p-pa;

        if(p>270 && pa<90) q-=360;
        if(pa>270 && p<90) q+=360;

        float spriteScreenX=q*((float)WINDOW_WIDTH/FOV);
        float spriteScreenY=(float)WINDOW_HEIGHT/2;

        //scale = sprite height = sprite width
        float scale=DISTANCE_TO_PROJ/(distanceToSprite);
        if (scale>20) scale=20;
        float scaledSize=SPRITE_SIZE*scale;

        int resolution=2;
        int notSeenCount=0;
        int outX=0;
        for (int x=0; x<scaledSize; x+=resolution) {
            outX=x;
            // Check if sprite is behind a wall
            int screenX = (x + spriteScreenX - (scaledSize / 2));

            int index=(int) screenX * 120 / WINDOW_WIDTH;

            if (distances[index] < distanceToSprite) {
                notSeenCount+=resolution;
                if((x+resolution)>=(scaledSize) && notSeenCount>=x){
                    s.seen=false;
                }
                continue;
            }

            if(index<120){
                s.seen=true;
            }


            for (int y=0; y<scaledSize; y+=resolution) {
                int screenY = (y +spriteScreenY- (scaledSize / 2));
                int pixelX=x/scale;
                int pixelY=y/scale;

                int pixel = (pixelY*32+pixelX)*3;
                int red = alien[pixel];
                int green = alien[pixel + 1];
                int blue = alien[pixel + 2];

                if(red==255 && blue==255) continue;
                Color color = {static_cast<unsigned char>(red),static_cast<unsigned char>(green),static_cast<unsigned char>(blue),255};

                Vector2 v1={static_cast<float>(screenX),static_cast<float>(screenY)};
                Vector2 v2={static_cast<float>(screenX),static_cast<float>(screenY+resolution)};
                DrawLineEx(v1,v2, resolution, color);
            }

        }
    }

}
void Raycast::drawRays3D(Player& player, Map& map){
    int mx=0, my=0, mp=0; //mx=wall x, my=wall y, mp=wall position from map
    int dof=0; //depth of view
    float rx=0.0, ry=0.0, ra=0.0; //ray (first) hit position (intercect) and ra = angle
    float xo=0.0, yo=0.0; //intercect position offset
    Color colorV=Color{12, 112, 173,255};
    Color colorH=Color{14, 130, 201,255};

    Color wallColor=BLACK;

    float distH=INFINITY;
    float hx=0.0;
    float hy=0.0;
    float distV=INFINITY;
    float vx=0.0;
    float vy=0.0;
    float distR=INFINITY;
    int textureTypeH=0;
    int textureTypeV=0;
    int textureType=0;

    int shade=0;

    //D1*30 means 30 deg which is the furthest ray from the player 
    ra=FixAng(player.getDeltaAngle()->angle-D1*30);

    //cast 60 rays (120 but each r is half deg)
    for (int r=0;r<120;r++){
        dof=0;
        distH=horizontalIntersection(player,map, mx, my, mp, dof,  rx,  ry,  ra,  xo,  yo, hx, hy, textureTypeH);
        distV=verticalIntersection(player,map, mx, my, mp, dof,  rx,  ry,  ra,  xo,  yo, vx, vy, textureTypeV);
        float MiniMapPlayerPosX=(player.getPos()->getPosX())*Game::miniMapScale+Game::miniMapOffset-Game::miniMapPlayerOffset;
        float MiniMapPlayerPosY=(player.getPos()->getPosY())*Game::miniMapScale+Game::miniMapOffset-Game::miniMapPlayerOffset;
        if(distH<distV){
            DrawLine(MiniMapPlayerPosX,MiniMapPlayerPosY, hx*Game::miniMapScale+Game::miniMapOffset+Game::miniMapRayOffset, hy*Game::miniMapScale+Game::miniMapOffset+Game::miniMapRayOffset, GREEN);
            distR=distH;
            rx=hx;
            ry=hy;
            textureType=textureTypeH;
            wallColor=colorH;
            shade=100;
        }else{
            DrawLine(MiniMapPlayerPosX, MiniMapPlayerPosY, vx*Game::miniMapScale+Game::miniMapOffset+Game::miniMapRayOffset, vy*Game::miniMapScale+Game::miniMapOffset+Game::miniMapRayOffset, GREEN);
            distR=distV;
            rx=vx;
            ry=vy;
            textureType=textureTypeV;
            wallColor=colorV;
            shade=0;
        }
        ra+=DEG_PER_COLUMN*DEG2RAD;
        if(ra<0) ra+=PI*2;
        if(ra>2*PI) ra-=PI*2;

        //-------Draw3D-------
        float da=FixAng(player.getDeltaAngle()->angle-ra);
        /*
         *--fixAngle-fish eye--
         *fix the effect by taking the right angle distance of the triangle
         *instead of h side distance of triangle
         *because the distance is far from the h side of the triangle
         */
        distR=distR*cos(da);
        distances[r]=distR;

        int lineHeight=(WALL_SIZE*DISTANCE_TO_PROJ)/distR;

        float textureYStep=32.0/(float)lineHeight;
        float textureYOffset=0;
        if(lineHeight>WINDOW_HEIGHT){ 
            textureYOffset=((lineHeight-WINDOW_HEIGHT)/2.0);
            lineHeight=WINDOW_HEIGHT;
        }
        int lineOffset=(WINDOW_HEIGHT/2)-(lineHeight>>1);

        float textureY=textureYOffset*textureYStep;
        float textureX;
        //TODO: understand this code.
        //--
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
        //--

        textureY+=textureType*32;
        int darkness=(textureType==1)?-32:0;
        Color color;
        for (int y=0 ;y<lineHeight;y++) {
            int pixel = ((int)textureY*32+(int)textureX)*3;
            int red = textures32[pixel];
            int green = textures32[pixel+1];
            int blue = textures32[pixel+2];
            color = Color{static_cast<unsigned char>(red+darkness),static_cast<unsigned char>(green+darkness),static_cast<unsigned char>(blue+darkness),255};
            //color=wallColor;
            DrawLineEx(Vector2{(float)r*8, (float)lineOffset+y-1},Vector2{(float)r*8,(float)lineOffset+y} , 8, color);

            textureY+=textureYStep;
        }
        //floor and ceiling-------
        for(int y=lineOffset+lineHeight;y<WINDOW_HEIGHT;y++){
            float dy=y-(WINDOW_HEIGHT/2.0);
            float beta=(player.getDeltaAngle()->angle-ra);
            float raFix=cos(FixAng(beta));//fix
            //floor-----
            float s=PLAYER_HEIGHT*DISTANCE_TO_PROJ/(dy);
            float d=s/raFix;
            textureX=player.getPos()->getPosX()+cos(ra)*d;
            textureY=player.getPos()->getPosY()+sin(ra)*d;
            int index=((int)(textureY/64.0)*map.getMapX()+(int)(textureX/64.0));

            int mp=map.getMapF()[index];
            int floorTextureType = mp * 32 * 32; // Use 32x32 textures

            // Adjust texture coordinates for 32x32 textures and scale to 64x64
            int scaledTextureX = ((int)(textureX / 2) & 31); // Divide by 2 to fit into 32x32 texture
            int scaledTextureY = ((int)(textureY / 2) & 31); // Divide by 2 to fit into 32x32 texture

            int pixel = (scaledTextureY * 32 + scaledTextureX) * 3 + floorTextureType * 3;
            int red = textures32[pixel];
            int green = textures32[pixel+1];
            int blue = textures32[pixel+2];
            /*
            int floorTextureType=(mp)*64*64;
            int pixel = (((int)(textureY)&63)*64 + ((int)(textureX) &63))*3+floorTextureType*3;
            int red = textures64[pixel];
            int green = textures64[pixel+1];
            int blue = textures64[pixel+2];
            */
            color = Color{static_cast<unsigned char>(red),static_cast<unsigned char>(green),static_cast<unsigned char>(blue),255};

            DrawLineEx(Vector2{(float)(r*8),(float)(y-1)},Vector2{(float)r*8,(float)y} , 8, color);

            //ceiling-----
            int ceilingTextureType=3*64*64;
            pixel = (((int)(textureY)&63)*64 + ((int)(textureX) &63))*3+ceilingTextureType*3;
            red = textures64[pixel];
            green = textures64[pixel+1];
            blue = textures64[pixel+2];
            color = Color{static_cast<unsigned char>(red),static_cast<unsigned char>(green),static_cast<unsigned char>(blue),255};

            DrawLineEx(Vector2{(float)(r*8),(float)(WINDOW_HEIGHT-(y-1))},Vector2{(float)r*8,(float)(WINDOW_HEIGHT-y)} , 8, color);
        }
    }
}

