#include "raylib.h"
#include "Game.hpp"
int main(void)
{
    // Initialization
    //const int screenWidth = 1024;
    //const int screenHeight = 512;
    const int screenWidth = 960;
    const int screenHeight = 640;
    Game *game=new Game();
    game->init((char *)"RAYCAST", screenWidth, screenHeight);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    //
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        game->update(GetFrameTime());
        game->render();
        /*
        int currentFPS = GetFPS();
        std::cout<<currentFPS<<std::endl;
        */
    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}
