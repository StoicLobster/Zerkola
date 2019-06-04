#ifndef ZERKOLA_INCLUDE_GAME_CONTROL_H_
#define ZERKOLA_INCLUDE_GAME_CONTROL_H_
//Includes
#include <Eigen/Dense>
#include <list>
#include <unordered_map>
#include <cassert>

/* Game Control
 * Defines constants used throughout the game.
 * Note: PHYS refers to physics coordinates (x >= 0 and y <= 0)
 * Note: WNDW refers to window coordinates (x >= 0 and y >= 0)
 */
namespace gc {

/*=== WINDOW ===*/

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int WINDOW_SCREEN_LOCATION_X = 400;
const int WINDOW_SCREEN_LOCATION_Y = 300;
const int WINDOW_MARGIN = 50;

/*=== GAME DEFINITION ===*/

const char* const GAME_TITLE = "Zerkola";
const int FPS = 60;
/* Max allowed duration of a frame [ms] */
const int MAX_FRAME_TIME_MS = 1000 / FPS;
const int PLAYER_X_START_OFFST_PHYS = 200;
const int PLAYER_Y_START_PHYS = -384;
/* Blue player always starts on left */
const int BLUE_PLAYER_START_POS_X_PHYS = WINDOW_MARGIN + PLAYER_X_START_OFFST_PHYS;
const int BLUE_PLAYER_START_POS_Y_PHYS = PLAYER_Y_START_PHYS;
/* Red player always starts on right */
const int RED_PLAYER_START_POS_X_PHYS = WINDOW_WIDTH - WINDOW_MARGIN - PLAYER_X_START_OFFST_PHYS;
const int RED_PLAYER_START_POS_Y_PHYS = PLAYER_Y_START_PHYS;
const short MAX_MISSILES_PER_PLAYER = 10;

/*=== BOARD SIZE (WINDOW COORDS) ===*/

const int BOARD_WNDW_LEFT = WINDOW_MARGIN;
const int BOARD_WNDW_RIGHT = WINDOW_WIDTH - WINDOW_MARGIN;
const int BOARD_WNDW_TOP = WINDOW_MARGIN;
const int BOARD_WNDW_BOTTOM = WINDOW_HEIGHT - WINDOW_MARGIN;
const int COLOR_MRGN_R = 0;
const int COLOR_MRGN_G = 0;
const int COLOR_MRGN_B = 0;
const int COLOR_MRGN_A = 255;
const int COLOR_BOARD_R = 150;
const int COLOR_BOARD_G = 150;
const int COLOR_BOARD_B = 150;
const int COLOR_BOARD_A = 255;

/*=== BOARD DIMENSIONS (WINDOW COORDS) ===*/

/* 2D Physics Global Coordinates Unit Vectors
 * All physics must occur in quadrant 4 (x >= 0 && y <= 0)
 * X_2D_PHYS: 2D X unit vector. Positive right
 * Y_2D_PHYS: 2D Y unit vector. Positive up.
 */
const Eigen::Vector2i X_2D_PHYS(1,0), Y_2D_PHYS(0,1);
/* 3D Physics Global Coordinates Unit Vectors
 * All physics must occur in quadrant 4 (x >= 0 && y <= 0)
 * X_3D_PHYS: 3D X unit vector. Positive right
 * Y_3D_PHYS: 3D Y unit vector. Positive up.
 * Z_3D_PHYS: 3D Z unit vector. Positive CCW.
 */
const Eigen::Vector3i X_3D_PHYS(1,0,0), Y_3D_PHYS(0,1,0), Z_3D_PHYS(0,0,1);

/*=== BOARD DIMENSIONS (PHYSICS COORDS) ===*/

/* Game board dimensions used during physics */
const int BOARD_PHYS_LEFT(BOARD_WNDW_LEFT), BOARD_PHYS_RIGHT(BOARD_WNDW_RIGHT), BOARD_PHYS_TOP(-1*BOARD_WNDW_TOP), BOARD_PHYS_BOTTOM(-1*BOARD_WNDW_BOTTOM);
/* Top left corner of board (physics coordinates) */
const Eigen::Vector2i BOARD_PHYS_TL(BOARD_PHYS_LEFT, BOARD_PHYS_TOP);
/* Bottom right corner of board (physics coordinates) */
const Eigen::Vector2i BOARD_PHYS_BR(BOARD_PHYS_RIGHT, BOARD_PHYS_BOTTOM);
/* Boundaries of board (physics coordinates) defined by pair of vectors <B0, Bm> */
const std::list<std::pair<Eigen::Vector2i, Eigen::Vector2i>> BOARD_PHYS_BOUNDARIES = {
	{BOARD_PHYS_TL, X_2D_PHYS}, //TL East
	{BOARD_PHYS_TL, Y_2D_PHYS}, //TL North	
	{BOARD_PHYS_BR, X_2D_PHYS}, //BR East
	{BOARD_PHYS_BR, Y_2D_PHYS} //BR North	
};

/*=== SPRITES ===*/

const double SPRITE_SCALE = 1.0;
const char* const SPRITE_ANIMATION_FILE_PATH = "content/sprites/sprites.png";

/*=== TANK BODY SPRITE ===*/

const int TANK_BODY_SPRITE_UPDATE_RATE_MS = 100;
const int TANK_BODY_NUMBER_SPRITE_ROLL_ANIMATIONS = 3;
const int TANK_BODY_SPRITE_WIDTH = 31;
const int TANK_BODY_SPRITE_HEIGHT = 25;
const int RED_TANK_BODY_SPRITE_START_X = 0;
const int RED_TANK_BODY_SPRITE_START_Y = 0;
const int BLUE_TANK_BODY_SPRITE_START_X = 0;
const int BLUE_TANK_BODY_SPRITE_START_Y = RED_TANK_BODY_SPRITE_START_Y + 2*TANK_BODY_SPRITE_HEIGHT;
/* Tank body center relative to upper left corner of sprite. Add to UL to get center */
const int TANK_BODY_CENTER_RELATIVE_TO_UL_X(15), TANK_BODY_CENTER_RELATIVE_TO_UL_Y(12);

/*=== TANK TURRET SPRITE ===*/

const int TANK_TURRET_SPRITE_WIDTH = 13;
const int TANK_TURRET_SPRITE_HEIGHT = 25;
const int RED_TANK_TURRET_SPRITE_START_X = 0;
const int RED_TANK_TURRET_SPRITE_START_Y = BLUE_TANK_BODY_SPRITE_START_Y + 2*TANK_BODY_SPRITE_HEIGHT;
const int BLUE_TANK_TURRET_SPRITE_START_X = 0;
const int BLUE_TANK_TURRET_SPRITE_START_Y = RED_TANK_TURRET_SPRITE_START_Y + TANK_TURRET_SPRITE_HEIGHT;
/* Tank turret center relative to upper left corner of sprite. Add to UL to get center */
const int TANK_TURRET_CENTER_RELATIVE_TO_UL_X(6), TANK_TURRET_CENTER_RELATIVE_TO_UL_Y(18);

/*=== TANK DYNAMICS ===*/

/* [m] Radius of collision for tank */
const double TANK_RAD_COL = 10;
/* [kg] Mass of tank */
const double TANK_MASS = 1000;
/* [kg*m^2] Moment of inertia of tank about z axis */
const double TANK_MOMENT_OF_INERTIA_Z = 500;
/* [N] Maximum forward force command */
const double TANK_BODY_MAX_FRWD_FRC = 20'000;
/* [N] Braking force command */
const double TANK_BODY_BRK_FRC = -100'000;
/* [N] Maximum reverse force command */
const double TANK_BODY_MAX_REV_FRC = -10'000;
/* [N/s] Force rate limit for a forward move command */
const double TANK_BODY_FRWD_FRC_RATE_LIMIT = 60'000;
/* [N/s] Force rate limit for a reverse move command */
const double TANK_BODY_REV_FRC_RATE_LIMIT = -30'000;
/* [N*m] Torque for a rotate command */
const double TANK_BODY_ROT_TRQ_CMND = 3'000;
/* [deg/s] Angular rotation speed of tank turret when commanded */
const double TANK_TURRET_ROT_SPD = 75;
/* [m/s] Maximum longitudinal linear velocity of tank body */
const double TANK_BODY_MAX_LONG_VEL = 80;
/* [m/s] Minimum longitudinal linear velocity of tank body */
const double TANK_BODY_MIN_LONG_VEL = -60;
/* [deg] Maximum angle (+/-) that turret can make with body direction */
const double TANK_TURRET_MAX_ANG = 130;
/* [s] Time for tank to shift gears (drive to reverse and vice versa) */
const double TANK_SHIFT_TIME = 1.5;

/*=== MISSILE SPRITE ===*/

const int MISSILE_SPRITE_UPDATE_RATE_MS = 100;
const int MISSILE_NUMBER_SPRITE_ANIMATIONS = 4;
const int MISSILE_SPRITE_WIDTH = 7;
const int MISSILE_SPRITE_HEIGHT = 14;
const int MISSILE_SPRITE_START_X = 0;
const int MISSILE_SPRITE_START_Y = BLUE_TANK_TURRET_SPRITE_START_Y + TANK_TURRET_SPRITE_HEIGHT;
/* missile center relative to upper left corner of sprite. Add to UL to get center */
const int MISSILE_CENTER_RELATIVE_TO_UL_X(3), MISSILE_CENTER_RELATIVE_TO_UL_Y(4);

/*=== MISSILE DYNAMICS ===*/

/* [m] Radius of collision for missile */
const double MISSILE_RAD_COL = 4;
/* [m/s] Missile speed */
const double MISSLE_SPEED = 150;
/* [m] Distance missile must travel before it becomes active (can cause collision) */
const long double MISSILE_ACTIVE_DIST = 50;
/* [m] Maximum distance a missile can travel before being deleted */
const long double MISSILE_MAX_DIST = 99999999;
/* [m] Tolerance distance to boundary for missile ricochet. Required to account for floating point error */
const int MISSILE_RICOCHET_TOL = 1;

/*=== ENVIRONMENT DYNAMICS ===*/

/* Coefficient of static friction of board surface */
const double SURF_STATIC_MU = 1.0;
/* Coefficient of kinetic friction of board surface */
const double SURF_KINETIC_MU = 0.4;
/* [m/s^2] Acceleration due to gravity */
const double g = 9.81;
/* [m/s] Speed threshold for rolling resistance to come into effect */
const double RR_SPEED_THRESH = 0.1;
/* [Nm] Rolling resistance of tracks */
const double RR_TRQ = 2000;
/* [N] Rolling resistance of tracks */
const double RR_FRC = 500;

/* Player / tank color
 * 0: RED
 * 1: BLUE
 */
typedef enum PlayerColor {
	PLAYER_COLOR_NONE,
	RED,
	BLUE,
	PLAYER_COLOR_MAX
} PlayerColor;

/* Player type (computer or human)
 * 0: PLAYER_TYPE_NONE
 * 1: COMPUTER_R2D2
 * 2: COMPUTER_SKYNET
 * 3: HUMAN
 * 4: NUM_PLAYER_TYPE
 */
typedef enum PlayerType {
	PLAYER_TYPE_NONE,
	COMPUTER_R2D2,
	COMPUTER_SKYNET,
	HUMAN,
	NUM_PLAYER_TYPE
} PlayerType;

/* Map of computer player types and setup messages */
const std::unordered_map<PlayerType,std::string> ComputerPlayerList {
	{COMPUTER_R2D2, "1: R2D2 (Easy)"}
};

/* Semantic linear directions
 * 0: LINEAR_DIRECTION_NONE
 * 1: FORWARD
 * 2: REVERSE
 * 3: NUM_LINEAR_DIRECTION
 */
typedef enum LinearDirection {
	LINEAR_DIRECTION_NONE,
	FORWARD,
	BACKWARD,
	NUM_LINEAR_DIRECTION
} LinearDirection;

/* Semantic angular directions
 * 0: ANGULAR_DIRECTION_NONE
 * 1: CCW (counter clockwise)
 * 2: CW (clockwise)
 * 3: NUM_ANGULAR_DIRECTION
 */
typedef enum AngularDirection {
	ANGULAR_DIRECTION_NONE,
	CCW,
	CW,
	NUM_ANGULAR_DIRECTION
} AngularDirection;

/* Semantic tank motion states
 * 0: STATIONARY
 * 1: PROPULSION_FORWARD
 * 2: BRAKING
 * 3: SHIFTING
 * 4: REVERSE
 * 5: NUM_TANK_MOTION_STATE
 */
typedef enum TankMotionState {
	STATIONARY,
	PROPULSION_FORWARD,
	BRAKING,
	SHIFTING,
	REVERSE,
	NUM_TANK_MOTION_STATE
} TankMotionState;

} //namespace gc

#endif //ZERKOLA_INCLUDE_GAME_CONTROL_H_