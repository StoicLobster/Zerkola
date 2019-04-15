#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_
#include <matplotlibcpp.h>
#include <geometry.h>
#include <vector>
#include <ncurses.h>
#include <list>
#include <math.h>

namespace plt = matplotlibcpp;
namespace zerkola {
	//Constants
	const double kDRAW_FREQ = 60; //draw frequency [Hz]
	const double kPLAYER_START_X = 75.0; //default starting x position for player
	const double kAI_START_X = 25.0; //default starting x position for AI
	const double kSTART_Y = 50.0; //default starting y position for both tanks
	const double kMISSLE_SPEED = 2; //default missile speed
	const double kTANK_LONG_SPEED = 1; //default tank speed
	const double kTANK_ROT_SPEED = 0.1; //default tank speed
	//Constants

	class Missile : public geometry::PlotObj {
		//Missile object
		private:
		//members
		const double kLENGTH = 2;
		const double kWIDTH = 1;
		double long_move_speed_; //Longitudinal speed that the object can more each frame
		//methods
		void rot2D_align(const Eigen::Vector2d& tank_dir); //rotates the missile to align with the tank direction (tank_dir)

		public:
		//constructors
		Missile();
		Missile(const double& x, const double& y, const double& spd, const Eigen::Vector2d& tank_dir);
		~Missile();
		//methods
		void Move(const bool& frwd); //Translates object in the corresonding direction by its longitudinal speed
	};

	class Tank : public geometry::PlotObj {
		//Tank object
		private:
		//members
		const double kLENGTH = 4;
		const double kWIDTH = 2;
		double long_move_speed_; //Longitudinal speed that the object can more each frame
		Eigen::Rotation2Dd rot_move_speed_; //Rotational speed that the object can more each frame

		public:
		//constructors
		Tank();
		Tank(const double& x, const double& y);
		~Tank();
		//methods
		void Move(const bool& frwd); //Translates object in the corresonding direction by its longitudinal speed
		void Rot2D(const bool& ccw); //Rotate object by the rotation speed in the directio indicated
		Missile* Fire(); //Fires a missle and returns pointer to it
	};

	class Zerkola {

		private:
		//members
		const double kNorthLimit_, kEastLimit_, kSouthLimit_, kWestLimit_; //Limits of game board in North, South, East, West
		const std::vector<double> kGameBoardBoundaryX_; //Limits of game board x axis
		const std::vector<double> kGameBoardBoundaryY_; //Limits of game board y axis
		long primary_fig_num_; //Number to access primary plot figure
		Tank tank_player; //Tank for the player
		Tank tank_AI; //Tank for the AI
		std::list<Missile*> missiles_; //List of active missiles in game

		//methods
		void init_plot(); //Initialize plot and format
		void player_input(); //Gets player input and updates player tank accordingly

		public:
		//constructors
		Zerkola();
		~Zerkola();
		//methods
		void Run();
	};

} // namespace zerkola
#endif // ZERKOLA_INCLUDE_ZERKOLA_H_
