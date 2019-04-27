#include <missile.h>

namespace missile {
Missile::Missile(): _ID(0), _long_move_speed(gc::MISSLE_SPEED) {};

Missile::Missile(const int& id, const double& x, const double& y, const double& spd, const Eigen::Vector2d& tank_dir): 
geometry::PlotObj(x,y), 
_ID(id),
_long_move_speed(gc::MISSLE_SPEED),
_travel_dist(0.0)
{
    _polygon.clear();
    double missile_width = 1, missile_length = 2;
    _polygon.emplace_back(_center.x()-missile_width/2.0,_center.y()-missile_length/2);
    _polygon.emplace_back(_center.x()-missile_width/2.0,_center.y()+missile_length/2);
    _polygon.emplace_back(_center.x()+missile_width/2.0,_center.y()+missile_length/2);
    _polygon.emplace_back(_center.x()+missile_width/2.0,_center.y()-missile_length/2);
    _polygon.emplace_back(_center.x()-missile_width/2.0,_center.y()-missile_length/2);
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
    if (_travel_dist >= gc::MISSILE_ACTIVE_DIST) _collision_active = true;
    //Define boundary with vectors
    std::list<std::pair<Eigen::Vector2d,Eigen::Vector2d>> boundaries;
    // <B0 , Bm>
    Eigen::Vector2d B0, Bm;
    B0(0) = 0;
    B0(1) = gc::NORTH_LIMIT;
    Bm(0) = 1;
    Bm(1) = 0;
    boundaries.push_back(std::make_pair(B0,Bm));
    B0(0) = gc::EAST_LIMIT;
    B0(1) = 0;
    Bm(0) = 0;
    Bm(1) = 1;
    boundaries.push_back(std::make_pair(B0,Bm));
    B0(0) = 0;
    B0(1) = gc::SOUTH_LIMIT;
    Bm(0) = 1;
    Bm(1) = 0;
    boundaries.push_back(std::make_pair(B0,Bm));
    B0(0) = gc::WEST_LIMIT;
    B0(1) = 0;
    Bm(0) = 0;
    Bm(1) = 1;
    boundaries.push_back(std::make_pair(B0,Bm));
    //Determine closest intersection point
    double intersect_dist = sqrt( pow((gc::NORTH_LIMIT-gc::SOUTH_LIMIT),2) + pow((gc::EAST_LIMIT-gc::WEST_LIMIT),2) );
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
        if ( (intersect_pt(0) <= gc::WEST_LIMIT) && (_dir(0) < 0) ) ricochet_dir(0) *= -1;
        if ( (intersect_pt(0) >= gc::EAST_LIMIT) && (_dir(0) > 0) ) ricochet_dir(0) *= -1;
        if ( (intersect_pt(1) <= gc::SOUTH_LIMIT) && (_dir(1) < 0) ) ricochet_dir(1) *= -1;
        if ( (intersect_pt(1) >= gc::NORTH_LIMIT) && (_dir(1) > 0) ) ricochet_dir(1) *= -1;
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
}