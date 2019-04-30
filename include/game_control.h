#ifndef ZERKOLA_INCLUDE_GAME_CONTROL_H_
#define ZERKOLA_INCLUDE_GAME_CONTROL_H_
/* Game Control
 * Defines constants used throughout the game.
 */
//Includes

namespace gc {
// Game definition
const char* const GAME_TITLE = "Zerkola";
const int FPS = 60;
const int MAX_FRAME_TIME_MS = 1000 / FPS; //Max time a frame is allowed to last [ms]
const int RED_PLAYER_START_POS_X = 200;
const int RED_PLAYER_START_POS_Y = 240;
const int BLUE_PLAYER_START_POS_X = 440;
const int BLUE_PLAYER_START_POS_Y = 240;
const int MAX_MISSILES_PER_PLAYER = 10;
// Board Size
const double WEST_LIMIT = 0;
const double EAST_LIMIT = 100;
const double SOUTH_LIMIT = 0;
const double NORTH_LIMIT = 100;
// Window size
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
// Sprites
const double SPRITE_SCALE = 1.0;
const char* const TANK_SPRITE_ANIMATION_LOCATION = "content/sprites/red_tank_2.png";
const int TANK_SPRITE_UPDATE_RATE_MS = 100;
const int TANK_SPRITE_WIDTH = 29;
const int TANK_SPRITE_HEIGHT = 31;
const int RED_TANK_SPRITE_START_X = 0;
const int RED_TANK_SPRITE_START_Y = 0;
const int BLUE_TANK_SPRITE_START_X = 0;
const int BLUE_TANK_SPRITE_START_Y = 0;
// Tank Dynamics
const double TANK_ACCEL_CMND = 0.1; //Acceleration that is commanded from a forward move command
const double TANK_DECEL_CMND = 0.5; //Deceleration that is commanded from a reverse move command
const double TANK_MAX_LONG_VEL = 1.0; //Maximum positive longitudinal velocity that tank can achieve
const double TANK_MIN_LONG_VEL = -1.0; //Maximum negative longitudinal velocity that tank can achieve
const double TANK_ROT_SPEED = 0.1; //Rotation speed of tank
// Missile Dynamics
const double MISSLE_SPEED = 2; //default missile speed
const long double MISSILE_ACTIVE_DIST = 5; //Distance missile must travel before it becomes active
const long double MISSILE_MAX_DIST = 999999; //Maximum distance a missile can travel before being deleted TODO

typedef enum PlayerType {
	HUMAN,
	AI_0_R2D2,
	AI_1_SKYNET
} PlayerType;

typedef enum Direction {
	FORWARD,
	REVERSE,
	ROTATE_LEFT,
	ROTATE_RIGHT
} Direction;

}

#endif