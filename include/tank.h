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
Eigen::Vector2d _body_center, _body_dir, _turrent_center, _turret_dir; //Position and direction of parts of the tank
Eigen::Vector2d _body_lin_v, _body_lin_a; //Linear velocity and acceleration of tank body
double _body_ang_v, _body_ang_a; //rotational velocity and acceleration of tank body (always in global z direction)
double _rotate_body_torque_cmnd, _translate_body_frc_cmnd; //torque command to turn the tank and force command to translate the tank
double _tractive_accel_limit; //Tractive acceleration limit of tank tracks with ground
bool _slip; //Indicates if tank is currently slipping
short _num_missiles; //number of missiles remaining to fire
bool _fire_this_turn, _move_this_turn; //controls if each of these actions was taken this turn

/* integrates (trapezoidal) to solve for state x k+1 (x_kp1)
 * x_k+1 = x_k dt/2*(x_dot_k + x_dot_k+1)
 */
template <typename T>
inline void _integrate(const double dt, 
    T& x_k,
    T& x_dot_k,
    const T& x_dot_kp1) const { x_k += dt/2*(x_dot_k + x_dot_kp1); x_dot_k = x_dot_kp1; }; 

void _move(const double dt, 
    const gc::LinearDirections translate_body_cmnd, 
    const gc::AngularDirections rotate_body_cmnd,
    const gc::AngularDirections rotate_turret_cmnd); //moves the tank with the provided commands

void _fire(std::list<missile::Missile*>& missiles); //Fires a missile (if able) and apends it to the list

public:
Tank();
~Tank();
Tank(gc::PlayerColor player_color);

inline void resetTurn() {_fire_this_turn = false; _move_this_turn = false; }; //Resets turn
virtual void turn(const input::Input& input); //Takes turn. Default is for human player with keyboard inputs

};

}

#endif //ZERKOLA_INCLUDE_TANK_H_