#ifndef ZERKOLA_INCLUDE_TANK_H_
#define ZERKOLA_INCLUDE_TANK_H_

#include <animated_sprite.h>
#include <input.h>
#include <game_control.h>
#include <Eigen/Dense>
#include <list>

namespace tank {

class Tank : public animated_sprite::AnimatedSprite {

protected:
Eigen::Vector2d _body_center, _body_dir, _turrent_center, _turret_dir, _body_lin_v, _body_lin_a; //Center location, direction, and linear kinematics of tank
double _body_rot_v, _body_rot_a; //rotational velocity and acceleration of tank body
double _rotate_body_torque_cmnd, _translate_body_frc_cmnd; //torque command to turn the tank and force command to translate the tank
short _num_missiles; //number of missiles remaining to fire
bool _fire_this_turn, _integrate_this_turn; //controls if each of these actions was taken this turn

void _integrate(const double dt, 
    const gc::LinearDirections translate_body_cmnd, 
    const gc::AngularDirections rotate_body_cmnd,
    const gc::AngularDirections rotate_turret_cmnd); //apply the motion commands and integrate the vehicle dynamics to determine new tank pose
void _fire(std::list<missile::Missile*>& missiles); //Fires a missile (if able) and apends it to the list

public:
Tank();
~Tank();
Tank(gc::PlayerColor player_color);

inline void resetTurn() {_fire_this_turn = false; _integrate_this_turn = false; }; //Resets turn
virtual void turn(const input::Input& input); //Takes turn. Default is for human player with keyboard inputs

};

}

#endif //ZERKOLA_INCLUDE_TANK_H_