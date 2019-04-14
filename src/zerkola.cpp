#include <zerkola.h>

namespace zerkola {
	//Tank
	Tank::Tank() {};

	Tank::Tank(const double& long_spd, const double& rot_spd): geometry::PlotObj(), long_move_speed_(long_spd), rot_move_speed_(rot_spd) {
		polygon_.clear();
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()-kLENGTH/2);
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()+kLENGTH/2);
		polygon_.emplace_back(CG_.x()+kWIDTH/2.0,CG_.y()+kLENGTH/2);
		polygon_.emplace_back(CG_.x()+kWIDTH/2.0,CG_.y()-kLENGTH/2);
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()-kLENGTH/2);
		//Assert that start and end points are the same
		assert((polygon_.front().x() == polygon_.back().x()) && (polygon_.front().y() == polygon_.back().y()));
	}
	
	void Tank::Rot2D(const bool& ccw) {
		Eigen::Rotation2Dd rot = rot_move_speed_;
		if (!ccw) rot = rot.inverse();
		dir_ = rot*dir_;
		dir_.normalize();
		for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
			Eigen::Vector2d v = (*it) - CG_;
			(*it) = rot*v + CG_;
		}
		return;
	}

	void Tank::Move(const bool& frwd) {
		//Confirm that dir_ is normalized
		dir_.normalize();
		double spd = long_move_speed_;
		if (!frwd) spd *= -1.0;
		Eigen::Vector2d mvmnt_vec = dir_* spd;
		CG_ += mvmnt_vec;
		for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
			(*it) += mvmnt_vec;
		}
		//TODO: Check if this would violate boundaries and adjust
		return;
	}
	//Tank

	//Missile
	//Missile

	//Zerkola
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

	void Zerkola::player_input() {
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
			//Get player input
			//Run AI move
			//Plot objects
			tank_player.Plot();
			//Plot			
			plt::pause(1/kDRAW_FREQ);
		}

		return;
	}

} // namespace zerkola