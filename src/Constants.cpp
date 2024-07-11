#pragma once
extern constexpr int WINDOW_WIDTH = 960; //projection width
extern constexpr int WINDOW_HEIGHT = 640; //projection height
extern constexpr int FOV = 60;
extern constexpr int HALF_FOV = 30;
extern constexpr int WALL_SIZE = 64;
extern constexpr int SPRITE_SIZE = 32;
extern constexpr int PLAYER_HEIGHT = 32;
extern constexpr float DEG_PER_COLUMN = 0.5;//0.0625 // = (FOV/WINDOW_WIDTH)
extern constexpr int DISTANCE_TO_PROJ = 832; // distance to projection plane = (WINDOW_WIDTH/2)/tan(HALF_FOV)
