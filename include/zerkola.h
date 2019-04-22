#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_
#include <matplotlibcpp.h>
#include <geometry.h>
#include <vector>
#include <ncurses.h>
#include <list>
#include <math.h>
#include <string>
#include <utility>

//#define DEBUG
#define DEBUG_FREEZE
#define DEBUG_RICOCHET

namespace plt = matplotlibcpp;
namespace zerkola {
	//Constants
	const double DRAW_FREQ = 100; //draw frequency [Hz]
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
		AI_0_SKYNET
	} AI_List;
	//Constants

	class Missile : public geometry::PlotObj {
		//Missile object
		private:
		//members
		const int _ID;
		const double _LENGTH = 2;
		const double _WIDTH = 1;
		double _long_move_speed; //Longitudinal speed that the object can more each frame
		long double _travel_dist; //cumulative distance travelled by missile
		//methods
		void _translate(const double& dist); //Translates object in the current direction by given distance
		void _rotate_align(const Eigen::Vector2d& dir_align); //rotates the missile to align with the given direction

		public:
		//constructors
		Missile();
		Missile(const int& id, const double& x, const double& y, const double& spd, const Eigen::Vector2d& tank_dir);
		~Missile();
		//methods
		void Move(); //Moves missile. If missile would collide with boundary, instead will calculate ricochet.
		inline int ID() const { return(_ID); };
	};

	class Tank : public geometry::PlotObj {
		//Tank object
		protected:
		//members
		const double _LENGTH = 4;
		const double _WIDTH = 2;
		bool _turn_taken; //indicate that turn was taken
		double _long_move_speed; //Longitudinal speed that the object can more each frame
		Eigen::Rotation2Dd _rot_move_speed; //Rotational speed that the object can more each frame
		//methods
		void _translate(const bool& frwd); //Translates object in the corresonding direction by its longitudinal speed
		void _rotate(const bool& ccw); //Rotate object by the rotation speed in the directio indicated
		void _fire(std::list<Missile*>& missiles); //Fires a missle and adds it to missile list

		public:
		//constructors
		Tank();
		Tank(const double& x, const double& y,const std::string& color);
		virtual ~Tank();
		//methods
		virtual void Turn(std::list<Missile*>& missiles); //Executes turn of the tank. This method should be overriden for AI classes. Default turn is for player. If a missile was fired, adds it to the list.
		inline void ResetTurn() { _turn_taken = false; }; //Simply resets turn taken tracker
	};

	class SkyNet : public Tank {
		//AI enemy
		public:
		SkyNet();
		SkyNet(const double& x, const double& y,const std::string& color);
		~SkyNet();
		void Turn(std::list<Missile*>& missiles);
	};

	class Zerkola {
		//Game class itself
		private:
		//members
		const std::vector<double> _game_board_boundary_x; //Limits of game board x axis
		const std::vector<double> _game_board_boundary_y; //Limits of game board y axis
		long _primary_fig_num; //Number to access primary plot figure
		Tank* _tank_player_A; //Tank for player A
		Tank* _tank_player_B; //Tank for player B
		std::list<Missile*> _missiles_player_A; //List of active missiles in game for player A
		std::list<Missile*> _missiles_player_B; //List of active missiles in game for player B
		std::list<std::list<Missile*>*> _missiles_all; //list of all missiles
		//methods
		void _init_plot(); //Initialize plot and format
		bool _debug_check_missiles() const; //used for debugging to check if a missile has left the boundary

		public:
		//constructors
		Zerkola();
		~Zerkola();
		//methods
		void Run();
	};

} // namespace zerkola
#endif // ZERKOLA_INCLUDE_ZERKOLA_H_
