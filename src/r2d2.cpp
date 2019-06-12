#include <r2d2.h>
#include <geometry.h>
#include <iostream>
#include <stdlib.h> //rand, srand
#include <time.h> //time

namespace r2d2 {

R2D2::R2D2() {};

R2D2::~R2D2() {};

R2D2::R2D2(graphics::Graphics* graphics_ptr, gc::PlayerColor player_color, std::list<missile::Missile*>* missiles_ptr):
    tank::Tank(graphics_ptr, player_color, missiles_ptr),

    _currentManeuver(ManeuverType::mtINIT),
    _translate_body_cmnd_prev(gc::LinearDirection::LINEAR_DIRECTION_NONE),
    _rotate_body_cmnd_prev(gc::AngularDirection::ANGULAR_DIRECTION_NONE),
    _rotate_turret_cmnd_prev(gc::AngularDirection::ANGULAR_DIRECTION_NONE)
{
    //Seed rand
    srand(time(NULL));
};

void R2D2::_turn() {
    if (_boarderProximityCheck()) _navigateManeuver();
    else if (_dangerCheck()) _evasiveManeuver();
    else _aggressiveManeuver();
    //Store commands for next loop
    _translate_body_cmnd_prev = _translate_body_cmnd;
    _rotate_body_cmnd_prev = _rotate_body_cmnd;
    _rotate_turret_cmnd_prev = _rotate_turret_cmnd;
    return;
};

void R2D2::_navigateManeuver() {
    if (_currentManeuver == ManeuverType::mtNAVIGATION) {
        _translate_body_cmnd = _translate_body_cmnd_prev;
        _rotate_body_cmnd = _rotate_body_cmnd_prev;
    } else {
        _currentManeuver = ManeuverType::mtNAVIGATION;
        //Slow down if going forward
        if (motionState() == gc::TankMotionState::PROPULSION_FORWARD) _translate_body_cmnd = gc::LinearDirection::BACKWARD;
        //Choose a random direction and start turning
        int rand_num = rand() % 2;
        if (rand_num) _rotate_body_cmnd = gc::AngularDirection::CCW;
        else _rotate_body_cmnd = gc::AngularDirection::CW;
    }
    return;
}

void R2D2::_evasiveManeuver() {
    if (_currentManeuver == ManeuverType::mtEVASIVE) {
        _translate_body_cmnd = _translate_body_cmnd_prev;
        _rotate_body_cmnd = _rotate_body_cmnd_prev;
    } else {
        _currentManeuver = ManeuverType::mtEVASIVE;
        _translate_body_cmnd = gc::LinearDirection::FORWARD;
    }
    return;
}

void R2D2::_aggressiveManeuver() {
    if (_currentManeuver == ManeuverType::mtAGGRESSIVE) {
        _rotate_turret_cmnd = _rotate_turret_cmnd_prev;
        _rotate_body_cmnd = _rotate_body_cmnd_prev;
    } else {
        // Check if enemy is targeted (intersection of turret and enemy)
        double lambda;
        Eigen::Vector2d I;
        if (geo::LineCircleIntersection(geo::Cast3D2Dd(center()), geo::Cast3D2Dd(dirTurret()), 
            geo::Cast3D2Dd(_enemyTarget->center()), gc::TANK_RAD_COL, lambda, I)) {
            // Fire!
            _fire();
        } else {
            // Determine turret angle rotation required to target enemy
            // Returned in [-pi, pi]
            double theta = geo::AngBetweenVecs(dirTurret(), (_enemyTarget->center() - center()).normalized());
            //std::cout << "Aggressive Maneuver Theta: " << theta << std::endl;
            if (theta > 0) {
                _rotate_turret_cmnd = gc::AngularDirection::CCW;
                if (theta >= M_PI_4) _rotate_body_cmnd = gc::AngularDirection::CCW;
                else _rotate_body_cmnd = gc::AngularDirection::ANGULAR_DIRECTION_NONE;
            } else {
                _rotate_turret_cmnd = gc::AngularDirection::CW;
                if (theta <= -1*M_PI_4) _rotate_body_cmnd = gc::AngularDirection::CW;
                else _rotate_body_cmnd = gc::AngularDirection::ANGULAR_DIRECTION_NONE;
            }
        }
    }

    return;
}

bool R2D2::_dangerCheck() const {
    return(_directMissileHitCheck());
}

bool R2D2::_directMissileHitCheck() const {
    for (auto missile : (*_missiles_ptr)) {
        Eigen::Vector2d I;
        double lambda;
        if (geo::LineCircleIntersection(missile->center(), missile->dir(), geo::Cast3D2Dd(center()), gc::TANK_RAD_COL+gc::MISSILE_RAD_COL, lambda, I)) {
            return(true);
        }
    }
    return(false);
}

bool R2D2::_boarderProximityCheck() const {
    Eigen::Vector2d intersect_pt;
    double intersect_dist;
    geo::BoundaryMinDist(geo::Cast3D2Dd(center()),geo::Cast3D2Dd(dir()),intersect_pt,intersect_dist);
    return(intersect_dist <= NAVIGATION_THRESHOLD);
}

void R2D2::_printState() const {
    //Print base tank state
    Tank::_printState();
    //Print R2D2 related fields
    std::cout << "===== R2D2 =====" << std::endl << std::endl;
    std::cout << "Maneuver Type: " << _currentManeuver << std::endl;
    std::cout << std::endl;
    return;
}

} //namespace r2d2

std::ostream& operator<<(std::ostream& os, const r2d2::ManeuverType& inpt) {
	switch (inpt) {
        case r2d2::ManeuverType::mtINIT:
			os << "INIT";
			break;
		case r2d2::ManeuverType::mtNAVIGATION:
			os << "NAVIGATION";
			break;
		case r2d2::ManeuverType::mtEVASIVE:
			os << "EVASIVE";
			break;
        case r2d2::ManeuverType::mtAGGRESSIVE:
			os << "AGGRESSIVE";
			break;
        case r2d2::ManeuverType::mtNONE:
			os << "NONE";
			break;
		default:
			os << "ERROR";
			break;
	}
    return os;
}