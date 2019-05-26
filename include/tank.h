#ifndef ZERKOLA_INCLUDE_TANK_H_
#define ZERKOLA_INCLUDE_TANK_H_
//Includes
#include <animated_sprite.h>
#include <input.h>
#include <graphics.h>
#include <game_control.h>
#include <Eigen/Dense>
#include <list>
#include <missile.h>

//#define DEBUG_TANK
#define DEBUG_TANK_FIRE
//#define VERBOSE_TANK

namespace tank {

class Tank : public animated_sprite::AnimatedSprite {

private:
gc::PlayerColor _color;
graphics::Graphics* _graphics_ptr;
input::Input* _input_ptr; //Inputs used for human players
std::list<missile::Missile*>* _missiles_ptr; //Pointer to missile list used in Zerkola
sprite::Sprite _turret; //Turret sprite object
/* Boody Fixed Unit Vectors
 * _l_body is always aligned with the tank body longitudinal direction
 * _t_body is always aligned with the tank body transverse direction
 * _l_turret is always aligned with the tank turret longitudinal direction
 * _t_turret is always aligned with the tank turret transverse direction
 * _k is always opposite in sign to global Z
 * Note that the vertical direction is always aligned with the global Z direction (no roll or pitch)
 */
Eigen::Vector3d _l_body, _t_body, _l_turret, _t_turret, _k;
Eigen::Vector3d _center; //Tank body and turret have same center (not sure how to get around this using SDL_RenderCopyEX...)
Eigen::Vector3d _body_lin_v, _body_lin_a, _body_ang_v, _body_ang_a; //Linear and angular velocity and acceleration of tank body
Eigen::Vector3d _body_lin_v_prev, _body_lin_a_prev, _body_ang_v_prev, _body_ang_a_prev; //Previous linear and angular velocity and acceleration of tank body
/* Control Commands
 * Torque and force commands to rotate and translate the tank respectively.
 * _rotate_body_torque_cmnd is always in global direction Z
 * _translate_body_frc_cmnd is always in direction _l
 * _rotate_turret_spd_cmnd is always in global direction Z
 */
double _rotate_body_torque_cmnd, _translate_body_frc_cmnd, _rotate_turret_spd_cmnd;
double _tractive_accel_limit_mag; //Magnitude of tractive acceleration limit of tank tracks with ground
bool _slip; //Indicates if tank is currently slipping
short _ammo; //number of missiles remaining to fire
bool _fire_this_turn, _move_this_turn; //controls if each of these actions was taken this turn

/* Virtual Functions
 *
 */
void _animationDone(std::string currentAnimation);
void _setupAnimations();

inline void _resetTurn() {_fire_this_turn = false; _move_this_turn = false; }; //Resets turn
template <typename T>
inline void _integrate(double dt_s, 
    T& x_k,
    const T& x_dot_k,
    const T& x_dot_kp1) const { x_k += dt_s*(x_dot_k + x_dot_kp1)/2; }; 
void _setPose(); //Sets pose in base Sprite class and transforms to screen coordinates (negate y)

protected:
void _move(double dt_ms, 
    gc::LinearDirections translate_body_cmnd, 
    gc::AngularDirections rotate_body_cmnd,
    gc::AngularDirections rotate_turret_cmnd); //moves the tank with the provided commands
void _fire(std::list<missile::Missile*>* missiles); //Fires a missile (if able) and apends it to the list
/* _turn
 * Virtual function.
 * Takes turn. Default is for human player with keyboard inputs
 */
virtual void _turn(double dt_ms); //TODO: Make this pure virtual and make Human class for actual player
/* Integration (Trapezoidal)
 * Solves for state x k+1 (x_kp1)
 * x_k+1 = x_k + dt*(x_dot_k + x_dot_k+1)/2
 */

public:
Tank();
virtual ~Tank();
Tank(graphics::Graphics* graphics_ptr, gc::PlayerColor player_color, std::list<missile::Missile*>* missiles_ptr, input::Input* input_ptr = nullptr);

void updateTank(double dt_ms); //Takes tank turn and does housekeeping
void drawTank(); //Draws all parts of the tank

};

}

#endif //ZERKOLA_INCLUDE_TANK_H_