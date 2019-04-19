#include <zerkola.h>

namespace zerkola {

	//Missile
	Missile::Missile(): long_move_speed_(MISSLE_SPEED) {};

	Missile::Missile(const double& x, const double& y, const double& spd, const Eigen::Vector2d& tank_dir): geometry::PlotObj(x,y), long_move_speed_(MISSLE_SPEED) {
		polygon_.clear();
		polygon_.emplace_back(CG_.x()-WIDTH/2.0,CG_.y()-LENGTH/2);
		polygon_.emplace_back(CG_.x()-WIDTH/2.0,CG_.y()+LENGTH/2);
		polygon_.emplace_back(CG_.x()+WIDTH/2.0,CG_.y()+LENGTH/2);
		polygon_.emplace_back(CG_.x()+WIDTH/2.0,CG_.y()-LENGTH/2);
		polygon_.emplace_back(CG_.x()-WIDTH/2.0,CG_.y()-LENGTH/2);
		//Assert that start and end points are the same
		assert((polygon_.front().x() == polygon_.back().x()) && (polygon_.front().y() == polygon_.back().y()));
		//Align missile with tank
		rotate_align(tank_dir);
		return;
	};

	Missile::~Missile() {};

	void Missile::translate() {
		//Confirm that dir_ is normalized
		dir_.normalize();
		Eigen::Vector2d mvmnt_vec = dir_* long_move_speed_;
		CG_ += mvmnt_vec;
		for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
			(*it) += mvmnt_vec;
		}
		return;
	}

	void Missile::rotate_align(const Eigen::Vector2d& dir) {
		//Calculate required angle of rotation between missile direction and desired direction
		double dot, det, ang;
		dot = dir_.dot(dir);
		det = dir_.x()*dir.y() - dir_.y()*dir.x();
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
		//translate missile
		translate();
		//Check if this violates boundary
		geometry::LimCollision col_type = geometry::LimCollision::None;
		if (CheckBoundaryCollision(NorthLimit,EastLimit,SouthLimit,WestLimit,col_type)) {
			//Determine boundary vector to use for intersection calculation
			Eigen::Vector2d B0, Bm, ricochet_dir = dir_;
			switch (col_type) {
				case geometry::LimCollision::NorthCollision:
					B0(0) = 0.0;
					B0(1) = NorthLimit;
					Bm(0) = 1.0;
					Bm(1) = 0.0;
					ricochet_dir(1) *= -1;
					break;
				case geometry::LimCollision::EastCollision:
					B0(0) = EastLimit;
					B0(1) = 0.0;
					Bm(0) = 0.0;
					Bm(1) = 1.0;
					ricochet_dir(0) *= -1;
					break;
				case geometry::LimCollision::SouthCollision:
					B0(0) = 0.0;
					B0(1) = SouthLimit;
					Bm(0) = 1.0;
					Bm(1) = 0.0;
					ricochet_dir(1) *= -1;
					break;
				case geometry::LimCollision::WestCollision:
					B0(0) = WestLimit;
					B0(1) = 0.0;
					Bm(0) = 0.0;
					Bm(1) = 1.0;
					ricochet_dir(0) *= -1;
					break;
				case geometry::LimCollision::None:
					break;
			}

			//Calculate intersection point I and assign to CG if not parallel
			Eigen::Vector2d I;
			double lambda;
			if ((col_type != geometry::LimCollision::None) && (geometry::VectorIntersection(CG_,dir_,B0,Bm,lambda,I))) {
				//non-parallel
				//Perform ricochet
				CG_ = I;
				//Rotate missile to richochet direction
				rotate_align(ricochet_dir);
			}
		}
		return;
	}
	//Missile

	//Tank
	Tank::Tank() {};

	Tank::Tank(const double& x, const double& y,const std::string& color): geometry::PlotObj(x,y,color), long_move_speed_(TANK_LONG_SPEED), rot_move_speed_(TANK_ROT_SPEED) {
		polygon_.clear();
		polygon_.emplace_back(CG_.x()-WIDTH/2.0,CG_.y()-LENGTH/2);
		polygon_.emplace_back(CG_.x()-WIDTH/2.0,CG_.y()+LENGTH/2);
		polygon_.emplace_back(CG_.x()+WIDTH/2.0,CG_.y()+LENGTH/2);
		polygon_.emplace_back(CG_.x()+WIDTH/2.0,CG_.y()-LENGTH/2);
		polygon_.emplace_back(CG_.x()-WIDTH/2.0,CG_.y()-LENGTH/2);
		//Assert that start and end points are the same
		assert((polygon_.front().x() == polygon_.back().x()) && (polygon_.front().y() == polygon_.back().y()));
	}

	Tank::~Tank() {};
	
	void Tank::rotate(const bool& ccw) {
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

	void Tank::translate(const bool& frwd) {
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
		Missile* mssl_ptr = new Missile(CG_.x(),CG_.y(),MISSLE_SPEED,dir_);
		return (mssl_ptr);
	}

	void Tank::Turn(std::list<Missile*>& missiles) {
		//Read player input
		switch (getch()) {
			case '\033': 
				//Escape char used for arrow keys
				getch(); // skip [
				switch (getch()) {
					case 'A':
						//printw("You pressed Up!\n");
						translate(true);
						break;
					case 'B':
						//printw("You pressed Down!\n");
						translate(false);
						break;
					case 'C':
						//printw("You pressed Right!\n");
						rotate(false);
						break;
					case 'D':
						//printw("You pressed Left!\n");
						rotate(true);
						break;
				}
				break;
			case ' ':
				//Space bar
				//printw("You pressed Space!\n");
				missiles.push_back(fire());
				break;
		}
		flushinp(); //Flush input buffer
		return;
	}
	//Tank

	//SkyNet
	SkyNet::SkyNet() {};

	SkyNet::SkyNet(const double& x, const double& y,const std::string& color): Tank(x, y, color) {};

	SkyNet::~SkyNet() {};

	void SkyNet::Turn(std::list<Missile*>& missiles) {
		return;
	}
	//SkyNet

	//Zerkola
	Zerkola::Zerkola(): NorthLimit_(100.0), EastLimit_(100.0), SouthLimit_(0.0), WestLimit_(0.0),
			GameBoardBoundaryX_{NorthLimit_,NorthLimit_,SouthLimit_,SouthLimit_,NorthLimit_},
			GameBoardBoundaryY_{WestLimit_,EastLimit_,EastLimit_,WestLimit_,WestLimit_},
			primary_fig_num_(0)
			{
				//Create base class for player A
				tank_player_A = new Tank(PLAYER_A_START_X,START_Y,PLAYER_A_COLOR);
				//TODO: Input selection for type of AI player
				tank_player_B = new SkyNet(PLAYER_B_START_X,START_Y,PLAYER_B_COLOR);
			};

	Zerkola::~Zerkola() {
		delete tank_player_A;
		delete tank_player_B;
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

	void Zerkola::Run() {
		//Setup ncurses for user input
		initscr();
		cbreak(); //disables required [enter] after each keyboard input
		noecho();
		scrollok(stdscr, TRUE);
		nodelay(stdscr, TRUE); //makes getch() non-blocking
		//Initialize plot
		init_plot();
		//Instantiate game pieces
		while (true) {
			//Reset plot
			plt::cla();
			plt::axis("off");
			plt::plot(GameBoardBoundaryX_,GameBoardBoundaryY_, "k");
			plt::tight_layout();
			//Run Player A turn
			tank_player_A->Turn(missiles_);
			//Run Player B turn
			tank_player_B->Turn(missiles_);
			//Plot tanks
			tank_player_A->Plot();
			tank_player_B->Plot();
			for (auto it = missiles_.begin(); it != missiles_.end(); ++it) {
				//Move missile
				(*it)->Move(NorthLimit_, EastLimit_, SouthLimit_, WestLimit_);	
				//TODO: Check for collisions with tanks
				//TODO: Arbitrate between missile plot or explosion plot			
				//Plot
				(*it)->Plot();
			}
			//Draw			
			plt::pause(1/DRAW_FREQ);
		}

		return;
	}

} // namespace zerkola