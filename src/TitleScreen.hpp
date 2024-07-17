#include "raylib.h"
class TitleScreen{
    private:
        Vector2 titleTextSize;
        Vector2 gameOverTextSize;
        float fontSize;
        char const * titleText;
        char const * gameOverText;
    public:
        TitleScreen();
        bool update();
        void render(bool dead);
};
