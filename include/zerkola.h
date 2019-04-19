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

namespace plt = matplotlibcpp;
namespace zerkola {
	//Constants
	const double DRAW_FREQ = 60; //draw frequency [Hz]
	const double PLAYER_A_START_X = 75.0; //default starting x position for player
	const double PLAYER_B_START_X = 25.0; //default starting x position for AI
	const double START_Y = 50.0; //default starting y position for both tanks
	const double MISSLE_SPEED = 2; //default missile speed
	const double TANK_LONG_SPEED = 1; //default tank speed
	const double TANK_ROT_SPEED = 0.1; //default tank speed
	const std::string PLAYER_A_COLOR = "b"; //Player A is blue
	const std::string PLAYER_B_COLOR = "r"; //Player B is red

	typedef enum AI_List {
		AI_SkyNet
	} AI_List;
	//Constants

	class Missile : public geometry::PlotObj {
		//Missile object
		private:
		//members
		const double LENGTH = 2;
		const double WIDTH = 1;
		double long_move_speed_; //Longitudinal speed that the object can more each frame
		//methods
		void translate(); //Translates object in the corresonding direction by its longitudinal speed
		void rotate_align(const Eigen::Vector2d& dir); //rotates the missile to align with the given direction

		public:
		//constructors
		Missile();
		Missile(const double& x, const double& y, const double& spd, const Eigen::Vector2d& tank_dir);
		~Missile();
		//methods
		void Move(const double& NorthLimit, const double& EastLimit, const double& SouthLimit, const double& WestLimit); //Moves missile. If missile would collide with boundary, instead will calculate ricochet.
	};

	class Tank : public geometry::PlotObj {
		//Tank object
		protected:
		//members
		const double LENGTH = 4;
		const double WIDTH = 2;
		double long_move_speed_; //Longitudinal speed that the object can more each frame
		//methods
		Eigen::Rotation2Dd rot_move_speed_; //Rotational speed that the object can more each frame
		void translate(const bool& frwd); //Translates object in the corresonding direction by its longitudinal speed
		void rotate(const bool& ccw); //Rotate object by the rotation speed in the directio indicated
		Missile* fire(); //Fires a missle and returns pointer to it

		public:
		//constructors
		Tank();
		Tank(const double& x, const double& y,const std::string& color);
		virtual ~Tank();
		//methods
		virtual void Turn(std::list<Missile*>& missiles); //Executes turn of the tank. This method should be overriden for AI classes. Default turn is for player. If a missile was fired, adds it to the list.

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
		const double NorthLimit_, EastLimit_, SouthLimit_, WestLimit_; //Limits of game board in North, South, East, West
		const std::vector<double> GameBoardBoundaryX_; //Limits of game board x axis
		const std::vector<double> GameBoardBoundaryY_; //Limits of game board y axis
		long primary_fig_num_; //Number to access primary plot figure
		Tank* tank_player_A; //Tank for player A
		Tank* tank_player_B; //Tank for player B
		std::list<Missile*> missiles_; //List of active missiles in game
		//methods
		void init_plot(); //Initialize plot and format

		public:
		//constructors
		Zerkola();
		~Zerkola();
		//methods
		void Run();
	};

} // namespace zerkola
#endif // ZERKOLA_INCLUDE_ZERKOLA_H_
