#pragma once
#include "Map.hpp"
#include "Vector2D.hpp"
class GameObject{
    private:
    public:
        float speed=0.5;
        float x,y=0;
        float distnaceFromPlayer;
        GameObject(float x, float y);
        ~GameObject();
        void update(Vector2D playerPos,Map &map);
        void render();
};
