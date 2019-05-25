#include <missile.h>

namespace missile {
Missile::Missile(): _ID(0) {};

Missile::~Missile() {};

Missile::Missile(graphics::Graphics& graphics, const int& id, const double& x, const double& y, const Eigen::Vector2d& tank_dir): 
animated_sprite::AnimatedSprite(
    graphics, 
    gc::SPRITE_ANIMATION_FILE_PATH, 
    gc::MISSILE_SPRITE_START_X, 
    gc::MISSILE_SPRITE_START_Y, 
    gc::MISSILE_SPRITE_WIDTH, 
    gc::MISSILE_SPRITE_HEIGHT,
    gc::MISSILE_SPRITE_UPDATE_RATE_MS),
_ID(id),
_travel_dist(0.0),
_collision_active(false)
{
    //Align missile with tank
    _rotate_align(tank_dir);
    return;
};

void Missile::_translate(const double& dist) {
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

void Missile::Move() {
    if (_travel_dist >= gc::MISSILE_ACTIVE_DIST) _collision_active = true;
    //Determine closest intersection point (init with max)
    double intersect_dist = sqrt( pow((gc::BOARD_MAX_Y-gc::BOARD_MIN_Y),2) + pow((gc::BOARD_MAX_X-gc::BOARD_MIN_X),2) );
    Eigen::Vector2d intersect_pt;
    for (const auto& bdry : gc::BOARD_BOUNDARIES) {
        Eigen::Vector2d I, B0, Bm;
        B0 = bdry.first.cast<double>();
        Bm = bdry.second.cast<double>();
        double lambda;
        if (geo::VectorIntersection(_center,_dir,B0,Bm,lambda,I)) {
            //non-parallel
            if ((lambda > 0) && (lambda < intersect_dist)) {
                intersect_dist = lambda;
                intersect_pt = I;
            }
        }
    }
    #ifdef DEBUG_RICOCHET
        printw("ID: %i, Center Point: (%f,%f), Direction (%f,%f), Intersect Distance: %f, Intersect Point: (%f,%f)\n", _ID, _center.x(), _center.y(), _dir.x(), _dir.y(), intersect_dist, intersect_pt.x(), intersect_pt.y());
    #endif
    //Determine if missile is colliding with boundary
    //Check if missile would begin colliding by end of this turn
    if ((intersect_dist - gc::MISSLE_SPEED) <= gc::MISSILE_RAD_COL) {
        //translate missile to intersection point
        _translate(intersect_dist);
        //Enforce that missile is exactly on boundary to remove any floating point error
        //Determine ricochet direction
        Eigen::Vector2d ricochet_dir = _dir;
        if (_center.x() < gc::BOARD_MIN_X) {
            _center.x() = gc::BOARD_MIN_X;
            ricochet_dir(0) *= -1;
        }
        if (_center.x() > gc::BOARD_MAX_X) {
            _center.x() = gc::BOARD_MAX_X;
            ricochet_dir(0) *= -1;
        }
        if (_center.y() < gc::BOARD_MIN_Y) {
            _center.x() = gc::BOARD_MIN_Y;
            ricochet_dir(1) *= -1;
        }
        if (_center.y() > gc::BOARD_MAX_Y) {
            _center.x() = gc::BOARD_MAX_Y;
            ricochet_dir(1) *= -1;
        }
        //Rotate missile to richochet direction
        _rotate_align(ricochet_dir);
        #ifdef DEBUG_RICOCHET
            printw("Ricochet\n");
        #endif
    } else {
         //Move
        _translate(gc::MISSLE_SPEED);	
    }  

    #ifdef DEBUG_RICOCHET
        printw("Final pos and dir: (%f,%f) (%f,%f)\n", _center.x(), _center.y(), _dir.x(), _dir.y());
    #endif	

    return;
}
//Missile
}