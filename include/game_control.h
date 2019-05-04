#ifndef ZERKOLA_INCLUDE_GAME_CONTROL_H_
#define ZERKOLA_INCLUDE_GAME_CONTROL_H_
/* Game Control
 * Defines constants used throughout the game.
 */
//Includes
#include <Eigen/Dense>

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
// Tank Sprites (Image Coordinate System)
const double SPRITE_SCALE = 1.0;
const char* const TANK_SPRITE_ANIMATION_FILE_PATH = "content/sprites/tank_sprites.png";
const int TANK_BODY_SPRITE_UPDATE_RATE_MS = 100;
const int TANK_BODY_NUMBER_SPRITE_ROLL_ANIMATIONS = 3;
const int TANK_BODY_SPRITE_WIDTH = 29;
const int TANK_BODY_SPRITE_HEIGHT = 23;
const int RED_TANK_BODY_SPRITE_START_X = 0;
const int RED_TANK_BODY_SPRITE_START_Y = 0;
const int BLUE_TANK_BODY_SPRITE_START_X = 0;
const int BLUE_TANK_BODY_SPRITE_START_Y = RED_TANK_BODY_SPRITE_START_Y + TANK_BODY_SPRITE_HEIGHT;
const int TANK_BODY_CENTER_RELATIVE_TO_UL_X = 14; //add to UL to get center
const int TANK_BODY_CENTER_RELATIVE_TO_UL_Y = 12; //add to UL to get center
const int TANK_TURRET_SPRITE_WIDTH = 11;
const int TANK_TURRET_SPRITE_HEIGHT = 25;
const int RED_TANK_TURRET_SPRITE_START_X = 0;
const int RED_TANK_TURRET_SPRITE_START_Y = BLUE_TANK_BODY_SPRITE_START_Y + TANK_BODY_SPRITE_HEIGHT;
const int BLUE_TANK_TURRET_SPRITE_START_X = 0;
const int BLUE_TANK_TURRET_SPRITE_START_Y = RED_TANK_TURRET_SPRITE_START_Y + TANK_TURRET_SPRITE_HEIGHT;
const int TANK_TURRET_CENTER_RELATIVE_TO_TURRET_UL_X = 5; //add to UL to get center
const int TANK_TURRET_CENTER_RELATIVE_TO_TURRET_UL_Y = 18; //add to UL to get center
const int TANK_TURRET_CENTER_RELATIVE_TO_BODY_CENTER_X = 0; //add to body center to get turret center
const int TANK_TURRET_CENTER_RELATIVE_TO_BODY_CENTER_Y = 1; //add to body center to get turret center
// Tank Dynamics
const double TANK_MASS = 1000; //Mass of tank
const double TANK_MOMENT_OF_INERTIA_Z = 200; //Moment of inertia of tank about z axis
const double TANK_BODY_FRWD_FRC_INC_CMND = 10; //Force that is incremented from a forward move command
const double TANK_BODY_REV_FRC_DEC_CMND = 50; //Force that is decremented from a reverse move command
const double TANK_BODY_ROT_TRQ_INC_CMD = 300; //Torque that is incremented for a CCW rotate command and decremented for CW
const double TANK_TURRET_ROT_SPD = 0.5; //Angular rotation speed of tank turret when commanded
const double TANK_BODY_MAX_FRWD_FRC = 300; //Maximum forward force command
const double TANK_BODY_MAX_REV_FRC = -500; //Maximum reverse force command
const double TANK_BODY_MAX_ROT_TRQ = 100; //Maximum rotation torque command
const double TANK_BODY_MAX_LONG_VEL = 10; //Maximum longitudinal linear velocity of tank body
const double TANK_BODY_MIN_LONG_VEL = -3; //Minimum longitudinal linear velocity of tank body
const double TANK_TURRET_MAX_ANG = 95; //Maximum angle (+/-) that turret can make with body direction
const double TANK_RAD_COL = 2; //Radius of collision for tank
const double TANK_ROLLING_RESIST_FRC = 100; //Rolling resistance of tracks
const double TANK_ROLLING_RESIST_TRQ = 50; //Rolling resistance of tracks
// Environment Dyncamis (Note global frame is in image coordinate system)
const Eigen::Vector2i X_2D(1,0); //Global X unit vector
const Eigen::Vector2i Y_2D(0,-1); //Global Y unit vector
const Eigen::Vector3i X_3D(1,0,0); //Global X unit vector
const Eigen::Vector3i Y_3D(0,-1,0); //Global Y unit vector
const Eigen::Vector3i Z_3D(0,0,-1); //Global Z unit vector
const double SURF_STATIC_MU = 0.8;
const double SURF_KINETIC_MU = 0.4;
const double g = 9.81;
// Missile Dynamics
const double MISSLE_SPEED = 2; //default missile speed
const long double MISSILE_ACTIVE_DIST = 5; //Distance missile must travel before it becomes active
const long double MISSILE_MAX_DIST = 999999; //Maximum distance a missile can travel before being deleted TODO

typedef enum PlayerColor {
	RED,
	BLUE
} PlayerColor;

typedef enum PlayerType {
	HUMAN,
	AI_0_R2D2,
	AI_1_SKYNET
} PlayerType;

typedef enum LinearDirections {
	LINEAR_NONE,
	FORWARD,
	REVERSE
} LinearDirections;

typedef enum AngularDirections {
	ANGULAR_NONE,
	CCW,
	CW
} AngularDirections;

}

#endif