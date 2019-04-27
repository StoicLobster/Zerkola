#ifndef ZERKOLA_INCLUDE_TANK_H
#define ZERKOLA_INCLUDE_TANK_H
//Includes
#include <game_control.h>
#include <geometry.h>
#include <missile.h>
#include <list>

namespace tank {
class Tank : public geometry::PlotObj {
    //Tank object
    protected:
    //members
    geometry::PlotObj _left_track, _right_track, _turret; //various additional objects that define the tank
    bool _fire_done, _translate_done, _rotate_done; //indicate that action was taken during turn
    double _long_move_speed; //Longitudinal speed that the object can more each frame
    Eigen::Rotation2Dd _rot_move_speed; //Rotational speed that the object can more each frame
    //methods
    void _translate(const bool& frwd); //Translates object in the corresonding direction by its longitudinal speed
    void _rotate(const bool& ccw); //Rotate object by the rotation speed in the directio indicated
    void _fire(std::list<missile::Missile*>& missiles); //Fires a missle and adds it to missile list

    public:
    //constructors
    Tank();
    Tank(const double& x, const double& y,const std::string& color);
    virtual ~Tank();
    //methods
    virtual void Turn(std::list<missile::Missile*>& missiles); //Executes turn of the tank. This method should be overriden for AI classes. Default turn is for player. If a missile was fired, adds it to the list.
    inline void ResetTurn() { _fire_done = false; _translate_done = false; _rotate_done = false; }; //Simply resets turn taken tracker
    void PlotTank() const; //Special plot command for tank which consists of multiple objects
};
}


#endif // ZERKOLA_INCLUDE_TANK_H