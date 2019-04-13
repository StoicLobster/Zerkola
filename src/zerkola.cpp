#include <zerkola.h>

namespace zerkola {

Zerkola::Zerkola(): kNorthLimit_(100.0), kEastLimit_(100.0), kSouthLimit_(0.0), kWestLimit_(0.0),
		kGameBoardBoundaryX_{kNorthLimit_,kNorthLimit_,kSouthLimit_,kSouthLimit_,kNorthLimit_},
		kGameBoardBoundaryY_{kWestLimit_,kEastLimit_,kEastLimit_,kWestLimit_,kWestLimit_},
		primary_fig_num_(0){};

Zerkola::~Zerkola() {};

void Zerkola::init_plot() {
	primary_fig_num_ = plt::figure();
	plt::axis("off");
	return;
}

void Zerkola::Run() {
	//Setup ncurses
	initscr();
	cbreak();
	noecho();
	scrollok(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	//Initialize plot
	init_plot();
	//Instantiate game pieces
	geometry::PlotObj tank_player(50,50,geometry::BoundaryPoly::kTank,1);
	while (true) {
		//Read input
		if (getch() == '\033') { // check for esc char
			getch(); // skip [
			switch (getch()) {
				case 'A':
					printw("You pressed Up!\n");
					tank_player.Move();
					break;
				case 'B':
					printw("You pressed Down!\n");
					break;
				case 'C':
					printw("You pressed Right!\n");
					break;
				case 'D':
					printw("You pressed Left!\n");
					break;
			}
		}
		//Clear plot
		plt::cla();
		plt::axis("off");
		plt::plot(kGameBoardBoundaryX_,kGameBoardBoundaryY_);
		plt::tight_layout();
		tank_player.Move();
		//Update plot
		tank_player.Plot();
		plt::pause(0.01);
	}

	return;
}

} // namespace zerkola

