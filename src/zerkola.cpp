#include <zerkola.h>

namespace plt = matplotlibcpp;
namespace zerkola {
Zerkola::Zerkola(): _player_won(false), _game_board_boundary_x{gc::NORTH_LIMIT,gc::NORTH_LIMIT,gc::SOUTH_LIMIT,gc::SOUTH_LIMIT,gc::NORTH_LIMIT},
		_game_board_boundary_y{gc::WEST_LIMIT,gc::EAST_LIMIT,gc::EAST_LIMIT,gc::WEST_LIMIT,gc::WEST_LIMIT},
		_primary_fig_num(0)
		{
			//Create base class for player A
			_tank_player_A = new tank::Tank(gc::PLAYER_A_START_X,gc::START_Y,gc::PLAYER_A_COLOR);
			//TODO: Input selection for type of AI player
			_tank_player_B = new skynet::SkyNet(gc::PLAYER_B_START_X,gc::START_Y,gc::PLAYER_B_COLOR);
			//Store lists of missiles
			_missiles_all.push_back(&_missiles_player_A);
			_missiles_all.push_back(&_missiles_player_B);
		};

Zerkola::~Zerkola() {
	delete _tank_player_A;
	delete _tank_player_B;
	for (auto it = _missiles_player_A.begin(); it != _missiles_player_A.end(); ++it) {
		delete (*it);
	}
	for (auto it = _missiles_player_B.begin(); it != _missiles_player_B.end(); ++it) {
		delete (*it);
	}
	return;
};

void Zerkola::_init_plot() {
	_primary_fig_num = plt::figure();
	plt::axis("off");
	return;
}

bool Zerkola::_debug_check_missiles() const {
	for (auto& missile_list : _missiles_all) {
		for (auto& missile : (*missile_list)) {
			Eigen::Vector2d missile_center = missile->center();
			if ( (missile_center.x() > (gc::EAST_LIMIT + 3)) ||
					(missile_center.x() < (gc::WEST_LIMIT - 3)) || 
					(missile_center.y() > (gc::NORTH_LIMIT + 3)) || 
					(missile_center.y() < (gc::SOUTH_LIMIT - 3)) ) {
						printw("Missile #%i is out of bounds: (%f,%f)",missile->ID(),missile_center.x(),missile_center.y());
						return (true);
					}
		}
	}
	return (false);
}

void Zerkola::Run() {
	//Determine pause duration
	double pause_dur = 1/gc::DRAW_FREQ;
	//Setup ncurses for user input
	initscr();
	cbreak(); //disables required [enter] after each keyboard input
	noecho();
	scrollok(stdscr, TRUE);
	nodelay(stdscr, TRUE); //makes getch() non-blocking
	//Initialize plot
	_init_plot();
	//Instantiate game pieces
	bool done = false;
	while (!done) {
		//Reset plot
		plt::cla();
		plt::axis("off");
		plt::plot(_game_board_boundary_x,_game_board_boundary_y, "k");
		plt::tight_layout();
		//Run Player A turn
		_tank_player_A->ResetTurn();
		_tank_player_A->Turn(_missiles_player_A);
		//Run Player B turn
		_tank_player_B->ResetTurn();
		_tank_player_B->Turn(_missiles_player_B);
		//Plot tanks
		_tank_player_A->PlotTank();
		_tank_player_B->PlotTank();
		for (auto& missile_list : _missiles_all) {
			for (auto& missile : (*missile_list)) {
				//printw("Missile #%i\n",mssl_cnt);
				//Move missile
				missile->Move();	
				//TODO: Check for collisions with tanks
				if (_tank_player_A->CheckCollision(*missile)) {
					done = true;
					_player_won = false;					
				}
				if (_tank_player_B->CheckCollision(*missile)) {
					done = true;
					_player_won = true;					
				}  
				//TODO: Arbitrate between missile plot or explosion plot			
				//Plot
				int zord = -1;
				missile->Plot(zord);
			}
		}
		//Update limits
		plt::xlim(gc::WEST_LIMIT - gc::PLT_MRGN, gc::EAST_LIMIT + gc::PLT_MRGN);
		plt::ylim(gc::SOUTH_LIMIT - gc::PLT_MRGN, gc::NORTH_LIMIT + gc::PLT_MRGN);
		#ifdef DEBUG_FREEZE
			//check if any missile is out of boundes
			if (_debug_check_missiles()) pause_dur = 86400; //1 day worth of debugging time :)
		#endif
		//Draw			
		plt::pause(pause_dur);
	}
	endwin();

	if (_player_won) {
		std::cout << "You won!" << std::endl;
	} else {
		std::cout << "You lost. Play again." << std::endl;
	}

	return;
}
} // namespace zerkola