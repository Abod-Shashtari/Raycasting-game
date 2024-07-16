#pragma once
#include "Vector2D.hpp"
extern const int WINDOW_WIDTH; //projection h
extern const int WINDOW_HEIGHT; //projection height
extern const int FOV;
extern const int HALF_FOV;
extern const int WALL_SIZE;
extern const int SPRITE_SIZE;
extern const int PLAYER_HEIGHT;
extern const float DEG_PER_COLUMN;//0.0625 // = (FOV/WINDOW_WIDTH)
extern const int DISTANCE_TO_PROJ; // distance to projection plane = (WINDOW_WIDTH/2)/tan(HALF_FOV)
extern Vector2D SPWANING_POS[]; // distance to projection plane = (WINDOW_WIDTH/2)/tan(HALF_FOV)
