#include "raylib.h"
#include "Game.hpp"
int main(void)
{
    // Initialization
    const int screenWidth = 1024;
    const int screenHeight = 512;
    Game *game=new Game();
    game->init((char *)"RAYCAST", screenWidth, screenHeight);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    //
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        game->update();
        game->render();
    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}
