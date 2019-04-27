#include <tank.h>

namespace tank {
Tank::Tank() {};

Tank::Tank(const double& x, const double& y,const std::string& color): 
    geometry::PlotObj(x,y,color), 		
    _left_track(x,y,"k"),
    _right_track(x,y,"k"),
    _turret(x,y,color),
    _fire_done(false), 
    _translate_done(false),
    _rotate_done(false),
    _long_move_speed(gc::TANK_LONG_SPEED), 
    _rot_move_speed(gc::TANK_ROT_SPEED) {
        //Define primary body polygon. This defines radius of collision
        double body_width = 2.0, body_length = 4;
        double body_right = _center.x()+body_width/2.0, body_left = _center.x()-body_width/2.0;
        double body_top = _center.y()+body_length/2, body_bottom = _center.y()-body_length/2;
        _polygon.clear();
        _polygon.emplace_back(body_left,body_bottom);
        _polygon.emplace_back(body_left,body_top);
        _polygon.emplace_back(body_right,body_top);
        _polygon.emplace_back(body_right,body_bottom);
        _polygon.emplace_back(body_left,body_bottom);
        //Assert that start and end points are the same
        assert((_polygon.front().x() == _polygon.back().x()) && (_polygon.front().y() == _polygon.back().y()));
        //Calculate radius of collision
        _calc_rad_collision();
        //Set to active for collisions
        _collision_active = true;

        std::vector<Eigen::Vector2d> poly;
        //Define left and right tracks
        double track_width = 0.4, track_delta_length = 1.0;		
        poly.clear();	
        poly.emplace_back(body_left,body_bottom+track_delta_length/2);
        poly.emplace_back(body_left-track_width,body_bottom+track_delta_length/2);
        poly.emplace_back(body_left-track_width,body_top-track_delta_length/2);
        poly.emplace_back(body_left,body_top-track_delta_length/2);
        poly.emplace_back(body_left,body_bottom+track_delta_length/2);
        _left_track.UpdatePolygon(poly);
        poly.clear();
        poly.emplace_back(body_right,body_bottom+track_delta_length/2);
        poly.emplace_back(body_right+track_width,body_bottom+track_delta_length/2);
        poly.emplace_back(body_right+track_width,body_top-track_delta_length/2);
        poly.emplace_back(body_right,body_top-track_delta_length/2);
        poly.emplace_back(body_right,body_bottom+track_delta_length/2);
        _right_track.UpdatePolygon(poly);

        //Define turret
        double turret_width = 0.6, turret_length = 5, front_frac = 0.8;
        poly.clear();
        poly.emplace_back(_center.x()-turret_width/2.0,_center.y()-turret_length*(1-front_frac));
        poly.emplace_back(_center.x()-turret_width/2.0,_center.y()+turret_length*front_frac);
        poly.emplace_back(_center.x()+turret_width/2.0,_center.y()+turret_length*front_frac);
        poly.emplace_back(_center.x()+turret_width/2.0,_center.y()-turret_length*(1-front_frac));
        poly.emplace_back(_center.x()-turret_width/2.0,_center.y()-turret_length*(1-front_frac));
        _turret.UpdatePolygon(poly);
    }

Tank::~Tank() {};

void Tank::_rotate(const bool& ccw) {
    if (_rotate_done) return;
    _rotate_done = true;
    Eigen::Rotation2Dd rot = _rot_move_speed;
    if (!ccw) rot = rot.inverse();
    //Move tank parts
    Rotate(rot,_center);
    _turret.Rotate(rot,_center);	
    _left_track.Rotate(rot,_center);
    _right_track.Rotate(rot,_center);
    return;
}

void Tank::_translate(const bool& frwd) {
    if (_translate_done) return;
    _translate_done = true;
    //Confirm that dir_ is normalized
    //XXX Assumes that all objects are in same direction
    _dir.normalize();
    double spd = _long_move_speed;
    if (!frwd) spd *= -1.0;
    Eigen::Vector2d mvmnt_vec = _dir* spd;
    //Move tank parts
    Translate(mvmnt_vec);
    _turret.Translate(mvmnt_vec);
    _left_track.Translate(mvmnt_vec);
    _right_track.Translate(mvmnt_vec);
    //TODO: Check if this would violate boundaries and adjust		
    return;
}

void Tank::_fire(std::list<missile::Missile*>& missiles) {
    if (_fire_done) return;
    _fire_done = true;
    int num_missiles = missiles.size();
    if (num_missiles >= gc::MAX_MISSILE_CNT_PER_PLAYER) return;
    missile::Missile* mssl_ptr = new missile::Missile(num_missiles,_center.x(),_center.y(),gc::MISSLE_SPEED,_dir);
    missiles.push_back(mssl_ptr);		
    return;
}

void Tank::Turn(std::list<missile::Missile*>& missiles) {
    //Read player input
    switch (getch()) {
        //Arrow keys below
        // case '\033': 
        // 	//Escape char used for arrow keys
        // 	getch(); // skip [
        // 	switch (getch()) {
        // 		case 'A':
        // 			//printw("You pressed Up!\n");
        // 			_translate(true);
        // 			break;
        // 		case 'B':
        // 			//printw("You pressed Down!\n");
        // 			_translate(false);
        // 			break;
        // 		case 'C':
        // 			//printw("You pressed Right!\n");
        // 			_rotate(false);
        // 			break;
        // 		case 'D':
        // 			//printw("You pressed Left!\n");
        // 			_rotate(true);
        // 			break;
        // 	}
        // 	break;
        case 'w':
        case 'W':
            _translate(true);
            break;
        case 'a':
        case 'A':
            _rotate(true);
            break;
        case 's':
        case 'S':
            _translate(false);
            break;
        case 'd':
        case 'D':
            _rotate(false);
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

void Tank::PlotTank() const {
    int zord = 0;
    //Plot tracks
    _right_track.Plot(zord);
    _left_track.Plot(zord);
    //Plot body
    Plot(++zord);
    //Plot turret
    _turret.Plot(++zord);
    return;
}
}