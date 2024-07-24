#include "GameObject.hpp"
#include "Player.hpp"
#include "raylib.h"
class TitleScreen{
    private:
        Vector2 titleTextSize;
        Vector2 gameOverTextSize;
        Vector2 gameOverScoreTextSize;
        float fontSize;
        char const * titleText;
        char const * gameOverText;
        char const * gameOverScoreText;
    public:
        TitleScreen();
        bool update(std::vector<GameObject>& eneimes,Player &player,int& score);
        void render(bool dead,int score, int highScore);
};
