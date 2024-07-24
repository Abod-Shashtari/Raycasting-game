#include "Constants.hpp"
#include "raylib.h"
#include "Game.hpp"
int main(void)
{
    // Initialization
    const int screenWidth = WINDOW_WIDTH;
    const int screenHeight = WINDOW_HEIGHT;
    Game *game=new Game();
    game->init((char *)"RAYCAST", screenWidth, screenHeight);

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    // Main game loop
    while (!WindowShouldClose()) //detect window close button
    {
        game->update(GetFrameTime());
        game->render(GetFrameTime());
    }

    CloseWindow();
    return 0;
}
