#ifndef ZERKOLA_INCLUDE_MISSILE_H
#define ZERKOLA_INCLUDE_MISSILE_H
//Includes
#include <game_control.h>
#include <geometry.h>
#include <list>

//#define DEBUG_RICOCHET

namespace missile {
class Missile : public geometry::PlotObj {
    //Missile object
    private:
    //members
    const int _ID;
    double _long_move_speed; //Longitudinal speed that the object can more each frame
    long double _travel_dist; //cumulative distance travelled by missile
    //methods
    void _translate(const double& dist); //Translates object in the current direction by given distance
    void _rotate_align(const Eigen::Vector2d& dir_align); //rotates the missile to align with the given direction

    public:
    //constructors
    Missile();
    Missile(const int& id, const double& x, const double& y, const double& spd, const Eigen::Vector2d& tank_dir);
    ~Missile();
    //methods
    void Move(); //Moves missile. If missile would collide with boundary, instead will calculate ricochet.
    inline int ID() const { return(_ID); };
};
}
#endif // ZERKOLA_INCLUDE_MISSILE_H