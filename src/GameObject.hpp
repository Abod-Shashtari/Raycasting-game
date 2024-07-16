#pragma once
#include "Map.hpp"
#include "Vector2D.hpp"
class GameObject{
    private:
    public:
        float speed;
        float x,y=0;
        float distnaceFromPlayer;
        GameObject(Vector2D pos,float speed);
        ~GameObject();
        void update(Vector2D playerPos,Map &map);
        void render();
};
