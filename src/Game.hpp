#pragma once
class Game{
    public:
        constexpr static const double miniMapScale=0.3;
        constexpr static const double miniMapOffset=10;
        void init(char* title,int width,int height);
        void update();
        void render();
};

