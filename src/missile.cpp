#include <missile.h>
#include <iostream>

namespace missile {

Missile::Missile(): _ID(0) {};

Missile::~Missile() {};

Missile::Missile(
    graphics::Graphics* graphics_ptr, 
    int id, 
    double x, 
    double y, 
    const Eigen::Vector2d& tank_dir): 

animated_sprite::AnimatedSprite(
    graphics_ptr, 
    gc::SPRITE_ANIMATION_FILE_PATH, 
    gc::MISSILE_SPRITE_START_X, 
    gc::MISSILE_SPRITE_START_Y, 
    gc::MISSILE_SPRITE_WIDTH, 
    gc::MISSILE_SPRITE_HEIGHT,
    gc::MISSILE_SPRITE_UPDATE_RATE_MS),

_ID(id),
_center(x,y),
_dir(tank_dir),
_travel_dist(0.0),
_collision_active(false)
{
    //Align missile with tank
    _rotate_align(tank_dir);
    //Setup and play animations
    _setupAnimations();
    playAnimation("Move");
    return;
};

void Missile::_animationDone(std::string currentAnimation) {}; //TODO?

void Missile::_setupAnimations() {
    _addAnimation(
        gc::MISSILE_NUMBER_SPRITE_ANIMATIONS, 
        gc::MISSILE_SPRITE_START_X, 
        gc::MISSILE_SPRITE_START_Y, 
        "Move", 
        gc::MISSILE_SPRITE_WIDTH, 
        gc::MISSILE_SPRITE_HEIGHT, 
        false);
    //TODO add explosion animations
    return;
}

void Missile::_translate(double dist) {
    //Confirm that dir_ is normalized
    _dir.normalize();
    Eigen::Vector2d mvmnt_vec = _dir* dist;
    _center += mvmnt_vec;
    _travel_dist += dist;
    return;
}

void Missile::_rotate_align(const Eigen::Vector2d& dir_align) {
    //Calculate required angle of rotation between missile direction and desired direction
    double ang;
    ang = geo::AngBetweenVecs(_dir, dir_align);
    Eigen::Rotation2Dd rot(ang);
    //Change direction
    _dir = rot*_dir;
    _dir.normalize();
    return;
}

void Missile::_setPose() {
    this->setDirection(_dir);
    Eigen::Vector2i c_body(gc::MISSILE_CENTER_RELATIVE_TO_UL_X, gc::MISSILE_CENTER_RELATIVE_TO_UL_Y);
    this->setCOR(c_body);
    int center_x = static_cast<int>(std::round(_center.x()));
    int center_y = static_cast<int>(std::round(-1*_center.y()));
    int body_UL_x = center_x - gc::MISSILE_CENTER_RELATIVE_TO_UL_X;    
    int body_UL_y = center_y - gc::MISSILE_CENTER_RELATIVE_TO_UL_Y;
    assert(body_UL_x >= 0);
    assert(body_UL_y >= 0);
    Eigen::Vector2i UL_body(body_UL_x, body_UL_y);
    this->setUL(UL_body);
    return;
}

void Missile::_move(double dt_ms) {
    if (dt_ms == 0.0) return;
    double dt_s = dt_ms/1000.0;
    //Determine closest intersection point (init with max)
    double intersect_dist = sqrt( pow((gc::BOARD_PHYS_TOP-gc::BOARD_PHYS_BOTTOM),2) + pow((gc::BOARD_PHYS_RIGHT-gc::BOARD_PHYS_LEFT),2) );
    Eigen::Vector2d intersect_pt;
    for (const auto& bdry : gc::BOARD_PHYS_BOUNDARIES) {
        Eigen::Vector2d I, B0, Bm;
        B0 = bdry.first.cast<double>();
        Bm = bdry.second.cast<double>();
        double lambda;
        if (geo::LineLineIntersection(_center,_dir,B0,Bm,lambda,I)) {
            //non-parallel
            if ((lambda > 0) && (lambda < intersect_dist)) {
                intersect_dist = lambda;
                intersect_pt = I;
            }
        }
    }
    #ifdef DEBUG_RICOCHET
        std::cout << "Start _move()" << std::endl;
        std::cout << "ID: " << _ID << ", Center Point: (" << _center.x() << "," << _center.y() << "), Direction: (" << _dir.x() << "," << _dir.y() << "), Intersect Distance: " << intersect_dist << ", Intersect Point: (" << intersect_pt.x() << "," << intersect_pt.y() << ")" << std::endl;
    #endif
    //Determine if missile is colliding with boundary
    //Check if missile would begin colliding by end of this turn
    if ((intersect_dist - gc::MISSLE_SPEED*dt_s) <= gc::MISSILE_RAD_COL) {
        //translate missile to intersection point
        _translate(intersect_dist);
        //Enforce that missile is exactly on boundary to remove any floating point error
        //Determine ricochet direction
        Eigen::Vector2d ricochet_dir = _dir;
        if (abs(_center.x() - gc::BOARD_PHYS_LEFT) <= gc::MISSILE_RICOCHET_TOL) {
            //Left Boundary
            _center.x() = gc::BOARD_PHYS_LEFT;
            ricochet_dir(0) = abs(ricochet_dir(0));
        }
        if (abs(gc::BOARD_PHYS_RIGHT - _center.x()) <= gc::MISSILE_RICOCHET_TOL) {
            //Right Boundary
            _center.x() = gc::BOARD_PHYS_RIGHT;
            ricochet_dir(0) = -1*abs(ricochet_dir(0));
        }
        if (abs(_center.y() - gc::BOARD_PHYS_BOTTOM) <= gc::MISSILE_RICOCHET_TOL) {
            //Bottom Boundary
            _center.y() = gc::BOARD_PHYS_BOTTOM;
            ricochet_dir(1) = abs(ricochet_dir(1));
        }
        if (abs(gc::BOARD_PHYS_TOP - _center.y()) <= gc::MISSILE_RICOCHET_TOL) {
            //Top Boundary
            _center.y() = gc::BOARD_PHYS_TOP;
            ricochet_dir(1) = -1*abs(ricochet_dir(1));
        }
        //Rotate missile to richochet direction
        _rotate_align(ricochet_dir);
        #ifdef DEBUG_RICOCHET
            std::cout << "Ricochet!" << std::endl;
        #endif
    } else {
         //Move
        _translate(gc::MISSLE_SPEED*dt_s);	
    }  

    #ifdef DEBUG_RICOCHET
        std::cout << "Final pos and dir: (" << _center.x() << "," << _center.y() << "), (" << _dir.x() << "," << _dir.y() << ")" << std::endl;
    #endif	

    return;
}

void Missile::updateMissile(double dt_ms) {
    if (_travel_dist >= gc::MISSILE_ACTIVE_DIST) _collision_active = true;
    //Update animatedSprite
    animated_sprite::AnimatedSprite::update(dt_ms);
    //Move missile
    this->_move(dt_ms);
    //Set pose in base class
    this->_setPose();
    return;
}


}