#pragma once
#include "Vector2D.hpp"
#include "DeltaAngle.hpp"
#include "Map.hpp"
#include "GameObject.hpp"
#include "raylib.h"
#include <vector>

class Player{
    private:
        Vector2D* pos;
        DeltaAngle* deltaAngle; //delta x,y,angle calculated from cos & sin of angle
        int speed;
        Sound gunShotWav;
    public:
        Player(float x, float y,int speed);
        ~Player();
        void resetPlayerAngle();
        void update(Map& map,std::vector<GameObject> &enemies,int& score);
        void render(Map& map,float deltaTime);
        Vector2D* getPos();
        DeltaAngle* getDeltaAngle();
};
