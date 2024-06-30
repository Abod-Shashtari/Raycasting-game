#pragma once
#include "Vector2D.hpp"
#include "DeltaAngle.hpp"
#include "Map.hpp"

class Player{
    private:
        Vector2D* pos;
        DeltaAngle* deltaAngle;
        float pdx,pdy,pa; //delta x,y,angle calced from cos & sin of angle
        int speed;
    public:
        Player(float x, float y,int speed);
        ~Player();
        void update(Map& map);
        void render(Map& map);
        Vector2D* getPos();
        DeltaAngle* getDeltaAngle();
};
