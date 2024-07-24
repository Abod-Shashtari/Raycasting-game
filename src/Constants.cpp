#include "Constants.hpp"
#include "Vector2D.hpp"
const int WINDOW_WIDTH = 960; //projection width
const int WINDOW_HEIGHT = 640; //projection height
const int FOV = 60;
const int HALF_FOV = 30;
const int WALL_SIZE = 64;
const int SPRITE_SIZE = 32;
const int PLAYER_HEIGHT = 32;
const float DEG_PER_COLUMN = 0.5;//0.0625 // = (FOV/WINDOW_WIDTH)
const int DISTANCE_TO_PROJ = 832; // distance to projection plane = (WINDOW_WIDTH/2)/tan(HALF_FOV)
Vector2D SPWANING_POS[]={Vector2D(96, WALL_SIZE*5+16),Vector2D(WALL_SIZE*5+16,96),Vector2D(WALL_SIZE*9-16,WALL_SIZE*5+16),Vector2D(WALL_SIZE*5+16,WALL_SIZE*9-16)};
float ALIEN_SPEED[]={1,1.5,2};
float ALIEN_MIN_SPAWN_TIME[]={1.2,1.6,1.4};
float ALIEN_MAX_SPAWN_TIME[]={2.2,1.8,1.5};
