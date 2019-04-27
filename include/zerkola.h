#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_
//Includes
#include <matplotlibcpp.h>
#include <game_control.h>
#include <tank.h>
#include <missile.h>
#include <ncurses.h>
#include <list>
#include <utility>
#include <skynet.h>
#include <iostream>

//#define DEBUG
#define DEBUG_FREEZE

namespace zerkola {
class Zerkola {
	//Game class itself
	private:
	//members
	bool _player_won;
	const std::vector<double> _game_board_boundary_x; //Limits of game board x axis
	const std::vector<double> _game_board_boundary_y; //Limits of game board y axis
	long _primary_fig_num; //Number to access primary plot figure
	tank::Tank* _tank_player_A; //Tank for player A
	tank::Tank* _tank_player_B; //Tank for player B
	std::list<missile::Missile*> _missiles_player_A; //List of active missiles in game for player A
	std::list<missile::Missile*> _missiles_player_B; //List of active missiles in game for player B
	std::list<std::list<missile::Missile*>*> _missiles_all; //list of all missiles
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
