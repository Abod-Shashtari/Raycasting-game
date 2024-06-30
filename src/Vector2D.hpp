#pragma once
#include "raylib.h"

class Vector2D{
    private:
        Vector2 vec;
    public:
        Vector2D(float x,float y);
        void setPosX(float x);
        void setPosY(float y);
        float getPosX();
        float getPosY();
};
