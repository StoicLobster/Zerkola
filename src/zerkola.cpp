#include <zerkola.h>

namespace zerkola {

	const double kDRAW_RATE = 0.001; //draw rate [s]

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
		cbreak(); //disables required [enter] after each keyboard input
		noecho();
		scrollok(stdscr, TRUE);
		nodelay(stdscr, TRUE); //makes getch() non-blocking
		//Initialize plot
		init_plot();
		//Instantiate game pieces
		geometry::PlotObj tank_player(50,50,geometry::BoundaryPoly::kTank,1.0,0.1);
		while (true) {
			//Read input			
			if (getch() == '\033') { // check for esc char
				getch(); // skip [
				switch (getch()) {
					case 'A':
						printw("You pressed Up!\n");
						tank_player.Move(true);
						break;
					case 'B':
						printw("You pressed Down!\n");
						tank_player.Move(false);
						break;
					case 'C':
						printw("You pressed Right!\n");
						tank_player.Rot2D(false);
						break;
					case 'D':
						printw("You pressed Left!\n");
						tank_player.Rot2D(true);
						break;
				}
			}
			flushinp(); //Flush input buffer
			//Clear plot
			plt::cla();
			plt::axis("off");
			plt::plot(kGameBoardBoundaryX_,kGameBoardBoundaryY_);
			plt::tight_layout();
			//Update plot
			tank_player.Plot();
			plt::pause(kDRAW_RATE);
		}

		return;
	}

} // namespace zerkola