#include <zerkola.h>

namespace zerkola {

	const double kDRAW_FREQ = 60; //draw frequency [Hz]

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
			//Clear plot
			plt::cla();
			plt::axis("off");
			plt::plot(kGameBoardBoundaryX_,kGameBoardBoundaryY_);
			plt::tight_layout();
			//Read player input	
			switch (getch()) {
				case '\033': 
					//Escape char used for arrow keys
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
					break;
				case ' ':
					//Space bar
					printw("You pressed Space!\n");
					break;
			}
			flushinp(); //Flush input buffer
			//Update plot
			tank_player.Plot();
			plt::pause(1/kDRAW_FREQ);
		}

		return;
	}

} // namespace zerkola