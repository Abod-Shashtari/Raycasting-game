#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include <vector>

#ifndef PI
#define PI 3.1415926535
#endif

#ifndef P2
#define P2 PI/2
#endif

#ifndef P3
#define P3 3*(PI/2) 
#endif

#ifndef D1
#define D1 0.0174533 //1 deg to rad
#endif

class Raycast{
    private:
        float distances[120];
    public:
        void drawRays3D(Player& player, Map& map);
        void drawSprite(Player& player, std::vector<GameObject> &enemies);
        float horizontalIntersection(Player& player,Map& map, int mx,int my,int mp,int dof,float rx,float ry,float ra,float xo,float yo, float& hx,float& hy, int& textureTypeH);
        float verticalIntersection(Player& player, Map& map, int mx,int my,int mp,int dof,float rx,float ry,float ra,float xo,float yo, float& vx,float& vy, int& textureTypeV);
};

