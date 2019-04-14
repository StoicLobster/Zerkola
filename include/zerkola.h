#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_
#include <matplotlibcpp.h>
#include <geometry.h>
#include <vector>
#include <ncurses.h>
#include <list>

namespace plt = matplotlibcpp;
namespace zerkola {
	//Constants
	const double kDRAW_FREQ = 60; //draw frequency [Hz]
	const double kPLAYER_START_X = 75.0; //default starting x position for player
	const double kAI_START_X = 25.0; //default starting x position for AI
	const double kSTART_Y = 50.0; //default starting y position for both tanks
	//Constants

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
		Tank(const double& long_spd, const double& rot_spd);
		~Tank();
		//methods
		void Rot2D(const bool& ccw); //Rotate object by the rotation speed in the directio indicated
		void Move(const bool& frwd); //Translates object in the corresonding direction by its longitudinal speed
		void Fire(); 
	};

	class Missile : public geometry::PlotObj {
		//Missile object
		private:

		public:
		//constructors
		Missile();
		Missile(const double& spd);
		~Missile();
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
