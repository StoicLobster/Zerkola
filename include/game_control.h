#ifndef ZERKOLA_INCLUDE_GAME_CONTROL_H_
#define ZERKOLA_INCLUDE_GAME_CONTROL_H_
//Includes
#include <string>
namespace gc {
const double DRAW_FREQ = 120; //draw frequency [Hz]
const double PLAYER_A_START_X = 75.0; //default starting x position for player
const double PLAYER_B_START_X = 25.0; //default starting x position for AI
const double START_Y = 50.0; //default starting y position for both tanks
const double MISSLE_SPEED = 2; //default missile speed
const double TANK_LONG_SPEED = 1; //default tank speed
const double TANK_ROT_SPEED = 0.1; //default tank speed
const int MAX_MISSILES_PER_PLAYER = 10;
const std::string PLAYER_A_COLOR = "b"; //Player A is blue
const std::string PLAYER_B_COLOR = "r"; //Player B is red
const int MAX_MISSILE_CNT_PER_PLAYER = 10;
const long double MISSILE_ACTIVE_DIST = 5; //Distance missile must travel before it becomes active
const long double MISSILE_MAX_DIST = 999999; //Maximum distance a missile can travel before being deleted TODO
//Board Boundaries
const double WEST_LIMIT = 0;
const double EAST_LIMIT = 100;
const double SOUTH_LIMIT = 0;
const double NORTH_LIMIT = 100;
const double PLT_MRGN = 0.01*(NORTH_LIMIT - SOUTH_LIMIT); //margins for plot

typedef enum AI_List {
	AI_0_R2D2,
	AI_1_ROBOCOP,
	AI_2_SKYNET
} AI_List;
}
#endif