#pragma once
class Game{
    public:
        constexpr static const double miniMapScale=0.2;
        constexpr static const double miniMapOffset=10;
        constexpr static const double miniMapRayOffset=-3;
        void init(char* title,int width,int height);
        void update(float deltaTime);
        void render();
};

