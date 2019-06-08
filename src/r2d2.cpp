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

    _enemyTarget(nullptr),
    _currentManeuver(ManeuverType::MANEUVER_TYPE_NONE),
    _translate_body_cmnd_prev(gc::LinearDirection::LINEAR_DIRECTION_NONE),
    _rotate_body_cmnd_prev(gc::AngularDirection::ANGULAR_DIRECTION_NONE),
    _rotate_turret_cmnd_prev(gc::AngularDirection::ANGULAR_DIRECTION_NONE)
{
    //Seed rand
    srand(time(NULL));
};

void R2D2::_turn() {
    //std::cout << "Current Maneuver: " << _currentManeuver << std::endl;
    //std::cout << "Current Motion State: " << motionState() << std::endl;
    //If (close to boarder)
        //Navigate action
    //Else If (tank in danger)
        //Evasive action
    //Else
        //Aggressive action
        
    if (_boarderProximityCheck()) {
        //std::cout << "Navigation Maneuver!" << std::endl;
        _navigateManeuver();
    } else if (_dangerCheck()) {
        //std::cout << "Evasive Maneuver!" << std::endl;
        //Evasive maneuver
        _evasiveManeuver();
    }
    //Store commands for next loop
    _translate_body_cmnd_prev = _translate_body_cmnd;
    _rotate_body_cmnd_prev = _rotate_body_cmnd;
    _rotate_turret_cmnd_prev = _rotate_turret_cmnd;
    return;
};

void R2D2::_navigateManeuver() {
    if (_currentManeuver == ManeuverType::NAVIGATION) {
        _translate_body_cmnd = _translate_body_cmnd_prev;
        _rotate_body_cmnd = _rotate_body_cmnd_prev;
    } else {
        _currentManeuver = ManeuverType::NAVIGATION;
        //Slow down if going forward
        if (motionState() == gc::TankMotionState::PROPULSION_FORWARD) _translate_body_cmnd = gc::LinearDirection::BACKWARD;
        //Choose a random direction and start turning
        int rand_num = rand() % 2;
        if (rand_num) _rotate_body_cmnd = gc::AngularDirection::CCW;
        else _rotate_body_cmnd = gc::AngularDirection::CW;
        std::cout << "Rand Nun: " << rand_num << std::endl;
        std::cout << "Rotate Command: " << _rotate_body_cmnd << std::endl;
    }
    return;
}

void R2D2::_evasiveManeuver() {
    if (_currentManeuver == ManeuverType::EVASIVE) {
        _translate_body_cmnd = _translate_body_cmnd_prev;
        _rotate_body_cmnd = _rotate_body_cmnd_prev;
    } else {
        _currentManeuver = ManeuverType::EVASIVE;
        _translate_body_cmnd = gc::LinearDirection::FORWARD;
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
    geo::BoundaryMinDist(geo::Cast3D2Dd(center()),geo::Cast3D2Dd(direction()),intersect_pt,intersect_dist);
    return(intersect_dist <= NAVIGATION_THRESHOLD);
}

}