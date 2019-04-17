#include <zerkola.h>

namespace zerkola {

	//Missile
	Missile::Missile(): long_move_speed_(kMISSLE_SPEED) {};

	Missile::Missile(const double& x, const double& y, const double& spd, const Eigen::Vector2d& tank_dir): geometry::PlotObj(x,y), long_move_speed_(kMISSLE_SPEED) {
		polygon_.clear();
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()-kLENGTH/2);
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()+kLENGTH/2);
		polygon_.emplace_back(CG_.x()+kWIDTH/2.0,CG_.y()+kLENGTH/2);
		polygon_.emplace_back(CG_.x()+kWIDTH/2.0,CG_.y()-kLENGTH/2);
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()-kLENGTH/2);
		//Assert that start and end points are the same
		assert((polygon_.front().x() == polygon_.back().x()) && (polygon_.front().y() == polygon_.back().y()));
		//Align missile with tank
		rot2D_align(tank_dir);
		return;
	};

	Missile::~Missile() {};

	void Missile::rot2D_align(const Eigen::Vector2d& tank_dir) {
		//Calculate required angle of rotation between missile direction and tank direction
		double dot, det, ang;
		dot = dir_.dot(tank_dir);
		det = dir_.x()*tank_dir.y() - dir_.y()*tank_dir.x();
		ang = atan2(det,dot);
		#ifdef DEBUG
			printw("Align angle: %f", ang);
		#endif
		Eigen::Rotation2Dd rot(ang);
		//Change direction
		dir_ = rot*dir_;
		dir_.normalize();
		//Perform rotation on polygon
		for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
			Eigen::Vector2d v = (*it) - CG_;
			(*it) = rot*v + CG_;
		}
		return;
	}

	void Missile::Move(const double& NorthLimit, const double& EastLimit, const double& SouthLimit, const double& WestLimit) {
		//Move missile
		//Confirm that dir_ is normalized
		dir_.normalize();
		Eigen::Vector2d mvmnt_vec = dir_* long_move_speed_;
		CG_ += mvmnt_vec;
		for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
			(*it) += mvmnt_vec;
		}
		//Check if this violates boundary
		geometry::LimCollision col_type = geometry::LimCollision::None;
		if (CheckBoundaryCollision(NorthLimit,EastLimit,SouthLimit,WestLimit,col_type)) {
			//Ricochet
			//Determine boundary vector to use
			Eigen::Vector2d B0, Bm;
			switch (col_type) {
				case geometry::LimCollision::NorthCollision:
					B0.x = 0.0;
					B0.y = NorthLimit;
					Bm.x = 1.0;
					Bm.y = 0.0;
					break;
				case geometry::LimCollision::EastCollision:
					B0.x = EastLimit;
					B0.y = 0.0;
					Bm.x = 0.0;
					Bm.y = 1.0;
					break;
				case geometry::LimCollision::SouthCollision:
					B0.x = 0.0;
					B0.y = SouthLimit;
					Bm.x = 1.0;
					Bm.y = 0.0;
					break;
				case geometry::LimCollision::WestCollision:
					B0.x = WestLimit;
					B0.y = 0.0;
					Bm.x = 0.0;
					Bm.y = 1.0;
					break;
			}

			//Calculate intersection point I
			Eigen::Vector2d I;
			double lambda;
			if ( geometry::VectorIntersection(CG_,dir_,B0,Bm,lambda,I) ) {
				//non-parallel

			}

			std::vector< std::pair< Eigen::Vector2d , Eigen::Vector2d > > Lims; //Parameteric vector equation for each limit L = [A] + [B]*t
			Lims.emplace_back(0,0,NorthLimit,1.0,0.0);
			Lims.emplace_back(EastLimit,0.0,0.0,1.0);
			Lims.emplace_back(WestLimit,0.0,0.0,1.0);
			Lims.emplace_back(0.0,SouthLimit,1.0,0.0);
			//Calculate intersection point of missile direction and boundary limits
		}
		return;
	}
	//Missile

	//Tank
	Tank::Tank() {};

	Tank::Tank(const double& x, const double& y,const std::string& color): geometry::PlotObj(x,y,color), long_move_speed_(kTANK_LONG_SPEED), rot_move_speed_(kTANK_ROT_SPEED) {
		polygon_.clear();
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()-kLENGTH/2);
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()+kLENGTH/2);
		polygon_.emplace_back(CG_.x()+kWIDTH/2.0,CG_.y()+kLENGTH/2);
		polygon_.emplace_back(CG_.x()+kWIDTH/2.0,CG_.y()-kLENGTH/2);
		polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()-kLENGTH/2);
		//Assert that start and end points are the same
		assert((polygon_.front().x() == polygon_.back().x()) && (polygon_.front().y() == polygon_.back().y()));
	}

	Tank::~Tank() {};
	
	void Tank::rot2D(const bool& ccw) {
		Eigen::Rotation2Dd rot = rot_move_speed_;
		if (!ccw) rot = rot.inverse();
		dir_ = rot*dir_;
		dir_.normalize();
		//Perform rotation on polygon
		for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
			Eigen::Vector2d v = (*it) - CG_;
			(*it) = rot*v + CG_;
		}
		return;
	}

	void Tank::move(const bool& frwd) {
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

	Missile* Tank::fire() {
		Missile* mssl_ptr = new Missile(CG_.x(),CG_.y(),kMISSLE_SPEED,dir_);
		return (mssl_ptr);
	}

	Missile* Tank::Turn() {
		Missile* mssl_ptr = nullptr;
		//Read player input
		switch (getch()) {
			case '\033': 
				//Escape char used for arrow keys
				getch(); // skip [
				switch (getch()) {
					case 'A':
						//printw("You pressed Up!\n");
						move(true);
						break;
					case 'B':
						//printw("You pressed Down!\n");
						move(false);
						break;
					case 'C':
						//printw("You pressed Right!\n");
						rot2D(false);
						break;
					case 'D':
						//printw("You pressed Left!\n");
						rot2D(true);
						break;
				}
				break;
			case ' ':
				//Space bar
				//printw("You pressed Space!\n");
				mssl_ptr = fire();
				break;
		}
		flushinp(); //Flush input buffer
		return(mssl_ptr);
	}
	//Tank

	//Zerkola
	Zerkola::Zerkola(): kNorthLimit_(100.0), kEastLimit_(100.0), kSouthLimit_(0.0), kWestLimit_(0.0),
			kGameBoardBoundaryX_{kNorthLimit_,kNorthLimit_,kSouthLimit_,kSouthLimit_,kNorthLimit_},
			kGameBoardBoundaryY_{kWestLimit_,kEastLimit_,kEastLimit_,kWestLimit_,kWestLimit_},
			primary_fig_num_(0),
			tank_player(kPLAYER_START_X,kSTART_Y,kPLAYER_COLOR),
			tank_AI(kAI_START_X,kSTART_Y,kAI_COLOR) 
			{};

	Zerkola::~Zerkola() {
		for (auto it = missiles_.begin(); it != missiles_.end(); ++it) {
			delete (*it);
		}
		return;
	};

	void Zerkola::init_plot() {
		primary_fig_num_ = plt::figure();
		plt::axis("off");
		return;
	}

	bool Zerkola::check_ricochet(Missile* missile) {
		bool ricochet = false;
		//Check if missile has collided with boundary
		ricochet = missile->CheckBoundaryCollision(kNorthLimit_, kEastLimit_, kSouthLimit_, kWestLimit_);
		if (ricochet) {
			//Calculate where
		}
		return(ricochet);
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
		while (true) {
			//Clear plot
			plt::cla();
			plt::axis("off");
			plt::plot(kGameBoardBoundaryX_,kGameBoardBoundaryY_, "k");
			plt::tight_layout();
			//Run Player turn
			tank_player.Turn();
			//Run AI turn
			//Plot objects
			tank_player.Plot();
			tank_AI.Plot();
			for (auto it = missiles_.begin(); it != missiles_.end(); ++it) {
				//Move missile tentatively
				(*it)->Move(true);
				//Check for ricochet
				check_ricochet(*it);				
				//Plot
				(*it)->Plot();
			}
			//Plot			
			plt::pause(1/kDRAW_FREQ);
		}

		return;
	}

} // namespace zerkola