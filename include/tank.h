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
//#define DEBUG_TANK_FIRE
//#define VERBOSE_TANK

namespace tank {

/* Base class (ADT) for all human and computer tank players */
class Tank : public animated_sprite::AnimatedSprite {

/*=== START PRIVATE ===*/
private:
    /* Color of tank */
    gc::PlayerColor _color;

    /* Pointer to graphics */
    graphics::Graphics* _graphics_ptr;

    /* Turret sprite object (part of tank) */
    sprite::Sprite _turret;

    /* Boody Fixed Unit Vectors
    * _l_body is always aligned with the tank body longitudinal direction
    * _t_body is always aligned with the tank body transverse direction
    * _l_turret is always aligned with the tank turret longitudinal direction
    * _t_turret is always aligned with the tank turret transverse direction
    * _k is always aligned with global physics Z axis (Z_3D_PHYS). No pitch or roll
    */
    Eigen::Vector3d _l_body, _t_body, _l_turret, _t_turret, _k;

    /* Tank body and turret have same center (not sure how to get around this using SDL_RenderCopyEX...) */
    Eigen::Vector3d _center;

    /* Linear and angular velocity and acceleration of tank body */
    Eigen::Vector3d _body_lin_v, _body_lin_a, _body_ang_v, _body_ang_a;

    /* Previous linear and angular velocity and acceleration of tank body (used for integration) */
    Eigen::Vector3d _body_lin_v_prev, _body_lin_a_prev, _body_ang_v_prev, _body_ang_a_prev;

    /* Control Commands
    * Torque and force commands to rotate and translate the tank respectively.
    * _rotate_body_torque_cmnd is always in global direction Z
    * _translate_body_frc_cmnd is always in direction _l
    * _rotate_turret_spd_cmnd is always in global direction Z
    */
    double _rotate_body_torque_cmnd, _translate_body_frc_cmnd, _rotate_turret_spd_cmnd;

    /* Magnitude of tractive acceleration limit of tank tracks with ground */
    double _tractive_accel_limit_mag;

    /* Indicates if tank is currently slipping (currently turned off) */
    bool _slip;

    /* Number of missiles remaining to fire */
    short _ammo;

    /* Controls if each of these actions was taken this turn */
    bool _fire_this_turn, _move_this_turn;

    /* Current motion state */
    gc::TankMotionState _motionState;

    /* Timer used to track shifting to / from reverse */
    double _shiftTimer;

    /* Timer and interval used for printing tank state */
    double _printTimer_ms, _printInterval_ms;

    /* Determine current semantic motion state */
    void _determineMotionState(double dt_s);

    /* Cleanup when animation over, not currently used */
    virtual void _animationDone(std::string currentAnimation);

    /* Stores sprite animations from sprite sheet */
    virtual void _setupAnimations();

    /* Resets turn */
    void _resetTurn();

    /* Integration (Trapezoidal)
    * Solves for state x_k+1 (x_kp1)
    * x_k+1 = x_k + dt*(x_dot_k + x_dot_k+1)/2
    */
    template <typename T>
    inline void _integrate(double dt_s, 
        T& x_k,
        const T& x_dot_k,
        const T& x_dot_kp1) const { x_k += dt_s*(x_dot_k + x_dot_kp1)/2; }; 

    /* Sets pose in base Sprite class and transforms to screen coordinates (negate y) */
    void _setPose();

    /* Print state of base Tank ADT*/
    virtual void _printState() const = 0;

/*=== END PRIVATE ===*/

/*=== START PROTECTED ===*/
protected:
    /* Pointer to list of all missiles currently in game */
    std::list<missile::Missile*>* _missiles_ptr;
    
    /* Motion command for linear body direction. Used in _move() */
    gc::LinearDirection _translate_body_cmnd;

    /* Motion command for angular direction. Used in _move() */
    gc::AngularDirection _rotate_body_cmnd;

    /* Moion command for angular turret direction. Used in _move() */
    gc::AngularDirection _rotate_turret_cmnd;

    /* Moves the tank given semantic commands */
    void _move(double dt_ms);

    /* Fires a missile (if able) and apends it to the list */
    void _fire(std::list<missile::Missile*>* missiles);
    
    /* Executes this frame's turn. Tank is allowed one set of move commands and one fire command. Pure virtual function 
     * Output shouild be the various motion commands:
     * _translate_body_cmnd
     * _rotate_body_cmnd
     * _rotate_turret_cmnd
     */
    virtual void _turn() = 0;

    /* Returns TRUE if a missile collided with the tank */
    bool _collisionCheck() const;

/*=== END PROTECTED ===*/

/*=== START PUBLIC ===*/
public:
    Tank();
    virtual ~Tank();
    Tank(graphics::Graphics* graphics_ptr, 
        gc::PlayerColor player_color, 
        std::list<missile::Missile*>* missiles_ptr);

    /* Takes tank turn, does housekeeping, and returns TRUE if a missile collides with the tank */
    bool update(double dt_ms, bool verbose=false);

    /* Draws all parts of the tank */
    void drawTank();

    /* Return center position of tank */
    inline Eigen::Vector3d center() const { return(_center); };

    /* Return direction of tank body */
    inline Eigen::Vector3d dir() const { return(_l_body); };

    /* Return direction of tank turret */
    inline Eigen::Vector3d dirTurret() const { return(_l_turret); };

    /* Return motion state of tank */
    inline gc::TankMotionState motionState() const { return(_motionState); };
/*=== END PUBLIC ===*/

}; //class Tank

} //namespace tank

#endif //ZERKOLA_INCLUDE_TANK_H_