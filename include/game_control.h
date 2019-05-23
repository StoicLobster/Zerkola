#ifndef ZERKOLA_INCLUDE_GAME_CONTROL_H_
#define ZERKOLA_INCLUDE_GAME_CONTROL_H_
/* Game Control
 * Defines constants used throughout the game.
 */
//Includes
#include <Eigen/Dense>
#include <list>

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
// Coordinate Systems
// Note these are used for physics. All physics must occur in quadrant 4 (x >= 0 && y <= 0)
const Eigen::Vector2i X_2D(1,0); //Global X unit vector
const Eigen::Vector2i Y_2D(0,1); //Global Y unit vector
const Eigen::Vector3i X_3D(1,0,0); //Global X unit vector
const Eigen::Vector3i Y_3D(0,1,0); //Global Y unit vector
const Eigen::Vector3i Z_3D(0,0,1); //Global Z unit vector
// Board Size
const double WINDOW_MARGIN = 50;
const double MIN_X = WINDOW_MARGIN;
const double MAX_X = WINDOW_WIDTH - WINDOW_MARGIN;
const double MIN_Y = -1*WINDOW_HEIGHT + WINDOW_MARGIN;
const double MAX_Y = -1*WINDOW_MARGIN;
// Store as vector list
// <B0 , Bm>
const Eigen::Vector2d B0_UL(MIN_X,MAX_Y);
const Eigen::Vector2d B0_LL(MAX_X,MIN_Y);
const std::list<const std::pair<const Eigen::Vector2d, const Eigen::Vector2d>> boundaries;

// Window
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int WINDOW_SCREEN_LOCATION_X = 400;
const int WINDOW_SCREEN_LOCATION_Y = 300;
// Tank Sprites (Image Coordinate System)
const double SPRITE_SCALE = 1.0;
const char* const SPRITE_ANIMATION_FILE_PATH = "content/sprites/sprites.png";
const int TANK_BODY_SPRITE_UPDATE_RATE_MS = 100;
const int TANK_BODY_NUMBER_SPRITE_ROLL_ANIMATIONS = 3;
const int TANK_BODY_SPRITE_WIDTH = 31;
const int TANK_BODY_SPRITE_HEIGHT = 25;
const int RED_TANK_BODY_SPRITE_START_X = 0;
const int RED_TANK_BODY_SPRITE_START_Y = 0;
const int BLUE_TANK_BODY_SPRITE_START_X = 0;
const int BLUE_TANK_BODY_SPRITE_START_Y = RED_TANK_BODY_SPRITE_START_Y + 2*TANK_BODY_SPRITE_HEIGHT;
const int TANK_BODY_CENTER_RELATIVE_TO_UL_X = 15; //add to UL to get center
const int TANK_BODY_CENTER_RELATIVE_TO_UL_Y = 12; //add to UL to get center
const int TANK_TURRET_SPRITE_WIDTH = 11;
const int TANK_TURRET_SPRITE_HEIGHT = 25;
const int RED_TANK_TURRET_SPRITE_START_X = 0;
const int RED_TANK_TURRET_SPRITE_START_Y = BLUE_TANK_BODY_SPRITE_START_Y + 2*TANK_BODY_SPRITE_HEIGHT;
const int BLUE_TANK_TURRET_SPRITE_START_X = 0;
const int BLUE_TANK_TURRET_SPRITE_START_Y = RED_TANK_TURRET_SPRITE_START_Y + TANK_TURRET_SPRITE_HEIGHT;
const int TANK_TURRET_CENTER_RELATIVE_TO_UL_X = 5; //add to UL to get center
const int TANK_TURRET_CENTER_RELATIVE_TO_UL_Y = 18; //add to UL to get center
// Tank Dynamics
const double TANK_RAD_COL = 30; //[m] Radius of collision for tank
const double TANK_MASS = 1000; //[kg] Mass of tank
const double TANK_MOMENT_OF_INERTIA_Z = 500; //[kg*m^2] Moment of inertia of tank about z axis
const double TANK_BODY_FRWD_FRC_RATE_LIMIT = 2000; //[N/s] Force rate limit for a forward move command
const double TANK_BODY_REV_FRC_RATE_LIMIT = -5000; //[N/s] Force rate limit for a reverse move command
const double TANK_BODY_ROT_TRQ_CMND = 2500; //[N*m] Torque for a rotate command
const double TANK_TURRET_ROT_SPD = 30; //[deg/s] Angular rotation speed of tank turret when commanded
const double TANK_BODY_MAX_FRWD_FRC = 6000; //[N] Maximum forward force command
const double TANK_BODY_MAX_REV_FRC = -10000; //[N] Maximum reverse force command
const double TANK_BODY_MAX_LONG_VEL = 30; //[m/s] Maximum longitudinal linear velocity of tank body
const double TANK_BODY_MIN_LONG_VEL = -10; //[m/s] Minimum longitudinal linear velocity of tank body
const double TANK_TURRET_MAX_ANG = 130; //[deg] Maximum angle (+/-) that turret can make with body direction
// Missile Sprite
const int MISSILE_SPRITE_UPDATE_RATE_MS = 100;
const int MISSILE_NUMBER_SPRITE_ANIMATIONS = 4;
const int MISSILE_SPRITE_WIDTH = 5;
const int MISSILE_SPRITE_HEIGHT = 11;
const int MISSILE_SPRITE_START_X = 0;
const int MISSILE_SPRITE_START_Y = BLUE_TANK_TURRET_SPRITE_START_Y + TANK_TURRET_SPRITE_HEIGHT;
const int MISSILE_CENTER_RELATIVE_TO_UL_X = 2; //add to UL to get center
const int MISSILE_CENTER_RELATIVE_TO_UL_Y = 3; //add to UL to get center
// Missile Dynamics
const double MISSILE_RAD_COL = 3; //Missile radius of collision
const double MISSLE_SPEED = 2; //default missile speed
const long double MISSILE_ACTIVE_DIST = 10; //Distance missile must travel before it becomes active
const long double MISSILE_MAX_DIST = 99999999; //Maximum distance a missile can travel before being deleted TODO
// Environment Dyncamis
const double SURF_STATIC_MU = 1.0;
const double SURF_KINETIC_MU = 0.4;
const double g = 9.81; //[m/s^2]
const double RR_SPEED_THRESH = 0.1; //[m/s] Speed threshold for rolling resistance to come into effect
const double RR_TRQ = 2000; //[Nm] Rolling resistance of tracks
const double RR_FRC = 500; //[N] Rolling resistance of tracks


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