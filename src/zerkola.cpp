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

	void Missile::translate(const double& dist) {
		//Confirm that dir_ is normalized
		dir_.normalize();
		Eigen::Vector2d mvmnt_vec = dir_* dist;
		CG_ += mvmnt_vec;
		for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
			(*it) += mvmnt_vec;
		}
		return;
	}

	void Missile::rotate_align(const Eigen::Vector2d& dir_align) {
		//Calculate required angle of rotation between missile direction and desired direction
		double dot, det, ang;
		dot = dir_.dot(dir_align);
		det = dir_.x()*dir_align.y() - dir_.y()*dir_align.x();
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

	void Missile::Move() {
		//Define boundary with vectors
		std::list<std::pair<Eigen::Vector2d,Eigen::Vector2d>> boundaries;
		// <B0 , Bm>
		Eigen::Vector2d B0, Bm;
		B0(0) = 0;
		B0(1) = NORTH_LIMIT;
		Bm(0) = 1;
		Bm(1) = 0;
		boundaries.push_back(std::make_pair(B0,Bm));
		B0(0) = EAST_LIMIT;
		B0(1) = 0;
		Bm(0) = 0;
		Bm(1) = 1;
		boundaries.push_back(std::make_pair(B0,Bm));
		B0(0) = 0;
		B0(1) = SOUTH_LIMIT;
		Bm(0) = 1;
		Bm(1) = 0;
		boundaries.push_back(std::make_pair(B0,Bm));
		B0(0) = WEST_LIMIT;
		B0(1) = 0;
		Bm(0) = 0;
		Bm(1) = 1;
		boundaries.push_back(std::make_pair(B0,Bm));
		//Determine closest intersection point
		double intersect_dist = sqrt( pow((NORTH_LIMIT-SOUTH_LIMIT),2) + pow((EAST_LIMIT-WEST_LIMIT),2) );
		Eigen::Vector2d intersect_pt;
		//printw("==START==\n");
		for (auto& bdry : boundaries) {
			//printw("==BDRY==\n");
			Eigen::Vector2d I;
			double lambda;
			if (geometry::VectorIntersection(CG_,dir_,bdry.first,bdry.second,lambda,I)) {
				//non-parallel
				//printw("CG point: ( %f , %f )\n", CG_(0), CG_(1));
				//printw("I point: ( %f , %f )\n", I(0), I(1));
				//printw("lambda: %f\n", lambda);
				//Eigen::Vector2d tmp = I - CG_;
				//double tmp_dist = tmp.norm();
				//printw("tmp_dist: %f\n", tmp_dist);
				if ((lambda > 0) && (lambda < intersect_dist)) {
					intersect_dist = lambda;
					intersect_pt = I;
				}
			}
		}
		//printw("==END==\n");
		//Determine if missile is colliding with boundary
		//printw("Intersect Distance: %f\n", intersect_dist);
		//printw("CG point: ( %f , %f )\n", CG_(0), CG_(1));
		//printw("Intersect point: ( %f , %f )\n", intersect_pt(0), intersect_pt(1));
		//Check if missile would begin colliding by end of this turn
		if ((intersect_dist -long_move_speed_) <= rad_collision_) {
			//translate missile to intersection point
			translate(intersect_dist);
			//printw("CG point: ( %f , %f )\n", CG_(0), CG_(1));
			//Determine ricochet direction
			Eigen::Vector2d ricochet_dir = dir_;
			if ((intersect_pt(0) == WEST_LIMIT) || (intersect_pt(0) == EAST_LIMIT)) ricochet_dir(0) *= -1;
			if ((intersect_pt(1) == SOUTH_LIMIT) || (intersect_pt(1) == NORTH_LIMIT)) ricochet_dir(1) *= -1;
			//Rotate missile to richochet direction
			rotate_align(ricochet_dir);
			//Translate again away from boundary
			translate(long_move_speed_);
		} else {
			//Normal move
			translate(long_move_speed_);
		}
		return;
	}
	//Missile

	//Tank
	Tank::Tank() {};

	Tank::Tank(const double& x, const double& y,const std::string& color): geometry::PlotObj(x,y,color), turn_taken_(false), long_move_speed_(TANK_LONG_SPEED), rot_move_speed_(TANK_ROT_SPEED) {
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
		if (turn_taken_) return;
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
		if (turn_taken_) return;
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

	void Tank::fire(std::list<Missile*>& missiles) {
		if (turn_taken_) return;
		Missile* mssl_ptr = new Missile(CG_.x(),CG_.y(),MISSLE_SPEED,dir_);
		missiles.push_back(mssl_ptr);
		return;
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
				fire(missiles);
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
	Zerkola::Zerkola(): GameBoardBoundaryX_{NORTH_LIMIT,NORTH_LIMIT,SOUTH_LIMIT,SOUTH_LIMIT,NORTH_LIMIT},
			GameBoardBoundaryY_{WEST_LIMIT,EAST_LIMIT,EAST_LIMIT,WEST_LIMIT,WEST_LIMIT},
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
			tank_player_A->ResetTurn();
			tank_player_A->Turn(missiles_);
			//Run Player B turn
			tank_player_B->ResetTurn();
			tank_player_B->Turn(missiles_);
			//Plot tanks
			tank_player_A->Plot();
			tank_player_B->Plot();
			//int mssl_cnt = 0;
			for (auto it = missiles_.begin(); it != missiles_.end(); ++it) {
				//printw("Missile #%i\n",mssl_cnt);
				//Move missile
				(*it)->Move();	
				//TODO: Check for collisions with tanks
				//TODO: Arbitrate between missile plot or explosion plot			
				//Plot
				(*it)->Plot();
				//++mssl_cnt;
			}
			//Update limits
			plt::xlim(WEST_LIMIT - PLT_MRGN, EAST_LIMIT + PLT_MRGN);
			plt::ylim( SOUTH_LIMIT - PLT_MRGN, NORTH_LIMIT + PLT_MRGN);
			//Draw			
			plt::pause(1/DRAW_FREQ);
		}

		return;
	}

} // namespace zerkola