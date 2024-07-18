#include "GameObject.hpp"
#include "Player.hpp"
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
        bool update(std::vector<GameObject>& eneimes,Player &player,int& score );
        void render(bool dead,int score);
};
