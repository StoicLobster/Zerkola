#include <tank.h>
#include <algorithm>

namespace tank {

Tank::Tank() {};

Tank::~Tank() {};

Tank::Tank(gc::PlayerColor player_color) {};

void Tank::_move(const double dt, 
    const gc::LinearDirections translate_body_cmnd, 
    const gc::AngularDirections rotate_body_cmnd,
    const gc::AngularDirections rotate_turret_cmnd) {
    //Check if move already occurred
    if (_move_this_turn) return;
    _move_this_turn = true;
    
    //Apply linear body motion commands
    if (translate_body_cmnd == gc::LinearDirections::FORWARD) {
        _translate_body_frc_cmnd += gc::TANK_BODY_FRWD_FRC_INC_CMND;
    } else if (translate_body_cmnd == gc::LinearDirections::REVERSE) {
        _translate_body_frc_cmnd -= gc::TANK_BODY_REV_FRC_DEC_CMND;
    }
    //Limit force command
    _translate_body_frc_cmnd = std::max( _translate_body_frc_cmnd, gc::TANK_BODY_MAX_REV_FRC ); //tank force limits
    _translate_body_frc_cmnd = std::min( _translate_body_frc_cmnd, gc::TANK_BODY_MAX_FRWD_FRC );

    //Apply rotational body motion commands
    if (rotate_body_cmnd == gc::AngularDirections::CCW) {
        _rotate_body_torque_cmnd += gc::TANK_BODY_ROT_TRQ_INC_CMD;
    } else if (rotate_body_cmnd == gc::AngularDirections::CW) {
        _rotate_body_torque_cmnd -= gc::TANK_BODY_ROT_TRQ_INC_CMD;
    }
    //Limit torque command
    _rotate_body_torque_cmnd = std::max( _rotate_body_torque_cmnd, -1*gc::TANK_BODY_MAX_ROT_TRQ ); //tank torque limits
    _rotate_body_torque_cmnd = std::min( _rotate_body_torque_cmnd, gc::TANK_BODY_MAX_ROT_TRQ );

    //Determine if slip is occurring
    double a_mag = (_body_long_lin_a + _body_lat_lin_a).norm();
    _slip = a_mag > _tractive_accel_limit;

    //Arbitrate slip dynamics
    if (!_slip) {
        _tractive_accel_limit = gc::g*gc::SURF_STATIC_FRICTION;

        //Determine acceleration of tank given new commands
        /** Longitudinal **/
        Eigen::Vector2d new_long_lin_accel, new_lat_lin_accel, new_total_lin_accel;
        double new_long_lin_a_mag = 0, new_ang_a_mag = 0, new_lat_lin_a_mag = 0;
        new_long_lin_a_mag = (_translate_body_frc_cmnd - gc::TANK_ROLLING_RESIST_FRC)/gc::TANK_MASS;       
        _body_dir.normalize();
        new_long_lin_accel = _body_dir*new_long_lin_a_mag;
        /** Lateral **/
        new_ang_a_mag = (_rotate_body_torque_cmnd - gc::TANK_ROLLING_RESIST_TRQ)/gc::TANK_MOMENT_OF_INERTIA_Z;
        //Integrate angular acceleration to solve for current angular speed
        _integrate(dt, _body_ang_v, _body_ang_a, new_ang_a_mag);
        _body_ang_a = new_ang_a_mag;
        //Integrate angular speed to solve for current direction
        //TODO
        //Calculate required centripetal acceleration (XXX with old linear velocity)
        new_lat_lin_a_mag = _body_ang_v*_body_lin_v.norm();
        //Calculate RHS perpendicular vector (RH rule with z pointed up)
        Eigen::Vector2d lat_dir(_body_dir.y(),-1*_body_dir.x());
        lat_dir.normalize();
        new_lat_lin_accel = lat_dir*new_lat_lin_a_mag;
        /** Total **/
        new_total_lin_accel = new_long_lin_accel + new_lat_lin_accel;
        Eigen::Vector2d curr_v = _body_lin_v;
        //Integrate linear acceleration to solve for linear velocity
        _integrate(dt, _body_lin_v, _body_lin_a, new_total_lin_accel);
        _body_lin_a = new_total_lin_accel;
        //Integrate linear velocity to solve for position        
        _integrate(dt, _body_center, curr_v, _body_lin_v);
        
    } else if (!_slip) {
        _tractive_accel_limit = gc::g*gc::SURF_KINETIC_FRICTION;
    } 

    




    new_long_lin_a_mag = std::max( new_long_lin_a_mag, -1*_tractive_accel_limit ); //tractive accel limits
    new_long_lin_a_mag = std::min( new_long_lin_a_mag, _tractive_accel_limit ); 




    /** ANGULAR BODY **/
    double new_body_ang_a, new_body_ang_v, rot_ang;
    //body angular acceleration
    new_body_ang_a = _rotate_body_torque_cmnd/gc::TANK_MOMENT_OF_INERTIA_Z;
    new_body_ang_a = std::max( new_body_ang_a, gc::TANK_BODY_MIN_LONG_ACCEL );
    new_body_ang_a = std::min( new_body_ang_a, gc::TANK_BODY_MAX_LONG_ACCEL );
    //body angular velocity
    new_body_ang_v += dt*(_body_ang_a + new_body_ang_a)/2.0;
    new_body_ang_v = std::max( new_body_ang_v, gc::TANK_BODY_MIN_LONG_VEL );
    new_body_ang_v = std::min( new_body_ang_v, gc::TANK_BODY_MAX_LONG_VEL );
    // //body angular position change
    // rot_ang = dt*(_body_ang_v + new_body_ang_v)/2.0;
    // //rotate body direction by angular position change
    // Eigen::Rotation2Dd rot(rot_ang);
    // _body_dir.normalize();
    // _body_dir = rot*_body_dir;
    // _body_dir.normalize();
    /** LINEAR BODY **/
    Eigen::Vector2d new_body_lin_a, new_body_lin_v;
    double new_body_lin_a_mag, new_boyd_lin_v_mag;
    //body linear acceleration
    new_body_lin_a_mag = _translate_body_frc_cmnd/gc::TANK_MASS;
    new_body_lin_a_mag = std::max( new_body_lin_a_mag, gc::TANK_BODY_MIN_LIN_ACCEL );
    new_body_lin_a_mag = std::min( new_body_lin_a_mag, gc::TANK_BODY_MAX_LIN_ACCEL );
    new_body_lin_a = _body_dir*new_body_lin_a_mag;
    //body linear velocity
    new_boyd_lin_v_mag = new_body_lin_v.norm() + dt*(_body_lin_a.norm() + new_body_lin_a_mag)/ 2.0;
    new_body_lin_v += dt*(_body_lin_a + new_body_lin_a)/2.0;
    new_body_lin_v = std::max( new_body_lin_v, gc::TANK_BODY_MIN_LIN_VEL );
    new_body_lin_v = std::min( new_body_lin_v, gc::TANK_BODY_MAX_LIN_VEL );
    //body position
    _body_center += dt*(_body_lin_v + new_body_lin_v)/2.0;
    //store new linear body kinematics
    _body_lin_a = new_body_lin_a;
    _body_lin_v = new_body_lin_v;
    

    //Determine new position of turret (check for angular limits)
    double turret_spd = 0.0;
    if (rotate_turret_cmnd == gc::AngularDirections::CCW) turret_spd = gc::TANK_TURRET_ROT_SPD;
    else if (rotate_turret_cmnd == gc::AngularDirections::CW) turret_spd = -1*gc::TANK_TURRET_ROT_SPD;

}

}