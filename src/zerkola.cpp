#include <zerkola.h>

namespace zerkola {

	//Missile
	Missile::Missile(): _ID(0), _long_move_speed(MISSLE_SPEED) {};

	Missile::Missile(const int& id, const double& x, const double& y, const double& spd, const Eigen::Vector2d& tank_dir): 
	geometry::PlotObj(x,y), 
	_ID(id),
	_long_move_speed(MISSLE_SPEED),
	_travel_dist(0.0)
	{
		_polygon.clear();
		_polygon.emplace_back(_center.x()-_WIDTH/2.0,_center.y()-_LENGTH/2);
		_polygon.emplace_back(_center.x()-_WIDTH/2.0,_center.y()+_LENGTH/2);
		_polygon.emplace_back(_center.x()+_WIDTH/2.0,_center.y()+_LENGTH/2);
		_polygon.emplace_back(_center.x()+_WIDTH/2.0,_center.y()-_LENGTH/2);
		_polygon.emplace_back(_center.x()-_WIDTH/2.0,_center.y()-_LENGTH/2);
		//Assert that start and end points are the same
		assert((_polygon.front().x() == _polygon.back().x()) && (_polygon.front().y() == _polygon.back().y()));
		//Calculate radius of collision
		_calc_rad_collision();
		//Align missile with tank
		_rotate_align(tank_dir);
		return;
	};

	Missile::~Missile() {};

	void Missile::_translate(const double& dist) {
		//Confirm that dir_ is normalized
		_dir.normalize();
		Eigen::Vector2d mvmnt_vec = _dir* dist;
		_center += mvmnt_vec;
		for (auto it = _polygon.begin(); it != _polygon.end(); ++it) {
			(*it) += mvmnt_vec;
		}
		_travel_dist += dist;
		return;
	}

	void Missile::_rotate_align(const Eigen::Vector2d& dir_align) {
		//Calculate required angle of rotation between missile direction and desired direction
		double dot, det, ang;
		dot = _dir.dot(dir_align);
		det = _dir.x()*dir_align.y() - _dir.y()*dir_align.x();
		ang = atan2(det,dot);
		#ifdef DEBUG
			printw("Align angle: %f", ang);
		#endif
		Eigen::Rotation2Dd rot(ang);
		//Change direction
		_dir = rot*_dir;
		_dir.normalize();
		//Perform rotation on polygon
		for (auto it = _polygon.begin(); it != _polygon.end(); ++it) {
			Eigen::Vector2d v = (*it) - _center;
			(*it) = rot*v + _center;
		}
		return;
	}

	void Missile::Move() {
		if (_travel_dist >= MISSILE_ACTIVE_DIST) _collision_active = true;
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
			if (geometry::VectorIntersection(_center,_dir,bdry.first,bdry.second,lambda,I)) {
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
		#ifdef DEBUG_RICOCHET
			printw("ID: %i, Center Point: (%f,%f), Direction (%f,%f), Intersect Distance: %f, Intersect Point: (%f,%f)\n", _ID, _center.x(), _center.y(), _dir.x(), _dir.y(), intersect_dist, intersect_pt.x(), intersect_pt.y());
		#endif
		//printw("==END==\n");
		//Determine if missile is colliding with boundary
		//printw("Intersect Distance: %f\n", intersect_dist);
		//printw("CG point: ( %f , %f )\n", CG_(0), CG_(1));
		//printw("Intersect point: ( %f , %f )\n", intersect_pt(0), intersect_pt(1));
		//Check if missile would begin colliding by end of this turn
		if ((intersect_dist - _long_move_speed) <= _rad_collision) {
			//translate missile to intersection point
			_translate(intersect_dist);
			#ifdef DEBUG_RICOCHET
				printw("Ricochet\n");
				printw("Post ric. translate center: (%f,%f)\n", _center.x(), _center.y());
			#endif
			//printw("CG point: ( %f , %f )\n", CG_(0), CG_(1));
			//Determine ricochet direction
			Eigen::Vector2d ricochet_dir = _dir;
			if ( (intersect_pt(0) <= WEST_LIMIT) && (_dir(0) < 0) ) ricochet_dir(0) *= -1;
			if ( (intersect_pt(0) >= EAST_LIMIT) && (_dir(0) > 0) ) ricochet_dir(0) *= -1;
			if ( (intersect_pt(1) <= SOUTH_LIMIT) && (_dir(1) < 0) ) ricochet_dir(1) *= -1;
			if ( (intersect_pt(1) >= NORTH_LIMIT) && (_dir(1) > 0) ) ricochet_dir(1) *= -1;
			//Rotate missile to richochet direction
			_rotate_align(ricochet_dir);
			#ifdef DEBUG_RICOCHET
				printw("Post ric. rotate dir: (%f,%f)\n", _dir.x(), _dir.y());
			#endif
			return;
		}
		//Move
		_translate(_long_move_speed);	
		#ifdef DEBUG_RICOCHET
			printw("Final pos and dir: (%f,%f) (%f,%f)\n", _center.x(), _center.y(), _dir.x(), _dir.y());
		#endif	
		return;
	}
	//Missile

	//Tank
	Tank::Tank() {};

	Tank::Tank(const double& x, const double& y,const std::string& color): geometry::PlotObj(x,y,color), _turn_taken(false), _long_move_speed(TANK_LONG_SPEED), _rot_move_speed(TANK_ROT_SPEED) {
		_polygon.clear();
		_polygon.emplace_back(_center.x()-_WIDTH/2.0,_center.y()-_LENGTH/2);
		_polygon.emplace_back(_center.x()-_WIDTH/2.0,_center.y()+_LENGTH/2);
		_polygon.emplace_back(_center.x()+_WIDTH/2.0,_center.y()+_LENGTH/2);
		_polygon.emplace_back(_center.x()+_WIDTH/2.0,_center.y()-_LENGTH/2);
		_polygon.emplace_back(_center.x()-_WIDTH/2.0,_center.y()-_LENGTH/2);
		//Assert that start and end points are the same
		assert((_polygon.front().x() == _polygon.back().x()) && (_polygon.front().y() == _polygon.back().y()));
		//Calculate radius of collision
		_calc_rad_collision();
		//Set to active for collisions
		_collision_active = true;
	}

	Tank::~Tank() {};
	
	void Tank::_rotate(const bool& ccw) {
		if (_turn_taken) return;
		_turn_taken = true;
		Eigen::Rotation2Dd rot = _rot_move_speed;
		if (!ccw) rot = rot.inverse();
		_dir = rot*_dir;
		_dir.normalize();
		//Perform rotation on polygon
		for (auto it = _polygon.begin(); it != _polygon.end(); ++it) {
			Eigen::Vector2d v = (*it) - _center;
			(*it) = rot*v + _center;
		}		
		return;
	}

	void Tank::_translate(const bool& frwd) {
		if (_turn_taken) return;
		_turn_taken = true;
		//Confirm that dir_ is normalized
		_dir.normalize();
		double spd = _long_move_speed;
		if (!frwd) spd *= -1.0;
		Eigen::Vector2d mvmnt_vec = _dir* spd;
		_center += mvmnt_vec;
		for (auto it = _polygon.begin(); it != _polygon.end(); ++it) {
			(*it) += mvmnt_vec;
		}
		//TODO: Check if this would violate boundaries and adjust		
		return;
	}

	void Tank::_fire(std::list<Missile*>& missiles) {
		if (_turn_taken) return;
		_turn_taken = true;
		int num_missiles = missiles.size();
		if (num_missiles >= MAX_MISSILE_CNT_PER_PLAYER) return;
		Missile* mssl_ptr = new Missile(num_missiles,_center.x(),_center.y(),MISSLE_SPEED,_dir);
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
						_translate(true);
						break;
					case 'B':
						//printw("You pressed Down!\n");
						_translate(false);
						break;
					case 'C':
						//printw("You pressed Right!\n");
						_rotate(false);
						break;
					case 'D':
						//printw("You pressed Left!\n");
						_rotate(true);
						break;
				}
				break;
			case ' ':
				//Space bar
				//printw("You pressed Space!\n");
				_fire(missiles);
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
	Zerkola::Zerkola(): _game_board_boundary_x{NORTH_LIMIT,NORTH_LIMIT,SOUTH_LIMIT,SOUTH_LIMIT,NORTH_LIMIT},
			_game_board_boundary_y{WEST_LIMIT,EAST_LIMIT,EAST_LIMIT,WEST_LIMIT,WEST_LIMIT},
			_primary_fig_num(0)
			{
				//Create base class for player A
				_tank_player_A = new Tank(PLAYER_A_START_X,START_Y,PLAYER_A_COLOR);
				//TODO: Input selection for type of AI player
				_tank_player_B = new SkyNet(PLAYER_B_START_X,START_Y,PLAYER_B_COLOR);
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
				if ( (missile_center.x() > (EAST_LIMIT + 3)) ||
				     (missile_center.x() < (WEST_LIMIT - 3)) || 
					 (missile_center.y() > (NORTH_LIMIT + 3)) || 
					 (missile_center.y() < (SOUTH_LIMIT - 3)) ) {
						 printw("Missile #%i is out of bounds: (%f,%f)",missile->ID(),missile_center.x(),missile_center.y());
						 return (true);
					 }
			}
		}
		return (false);
	}

	void Zerkola::Run() {
		//Determine pause duration
		double pause_dur = 1/DRAW_FREQ;
		//Setup ncurses for user input
		initscr();
		cbreak(); //disables required [enter] after each keyboard input
		noecho();
		scrollok(stdscr, TRUE);
		nodelay(stdscr, TRUE); //makes getch() non-blocking
		//Initialize plot
		_init_plot();
		//Instantiate game pieces
		while (true) {
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
			_tank_player_A->Plot();
			_tank_player_B->Plot();
			for (auto& missile_list : _missiles_all) {
				for (auto& missile : (*missile_list)) {
					//printw("Missile #%i\n",mssl_cnt);
					//Move missile
					missile->Move();	
					//TODO: Check for collisions with tanks
					if (_tank_player_A->CheckCollision(*missile)) printw("HIT!\n");
					//TODO: Arbitrate between missile plot or explosion plot			
					//Plot
					missile->Plot();
				}
			}

			//Update limits
			plt::xlim(WEST_LIMIT - PLT_MRGN, EAST_LIMIT + PLT_MRGN);
			plt::ylim( SOUTH_LIMIT - PLT_MRGN, NORTH_LIMIT + PLT_MRGN);
			#ifdef DEBUG_FREEZE
				//check if any missile is out of boundes
				if (_debug_check_missiles()) pause_dur = 86400; //1 day worth of debugging time :)
			#endif
			//Draw			
			plt::pause(pause_dur);
		}

		return;
	}

} // namespace zerkola