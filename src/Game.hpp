#pragma once
class Game{
    public:
        constexpr static double miniMapScale=0.2;
        constexpr static double miniMapOffset=10;
        constexpr static double miniMapRayOffset=-3;
        constexpr static double miniMapPlayerOffset=5.5;
        bool dead=false;
        void init(char* title,int width,int height);
        void update(float deltaTime);
        void render(float deltaTime);
};

