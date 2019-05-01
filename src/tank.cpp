#include <tank.h>

namespace tank {

Tank::Tank() {};

Tank::~Tank() {};

Tank::Tank(gc::PlayerColor player_color) {};

void Tank::_integrate(const double dt, 
    const gc::LinearDirections translate_body_cmnd, 
    const gc::AngularDirections rotate_body_cmnd,
    const gc::AngularDirections rotate_turret_cmnd) {
        //Check if integration already occurred
        if (_integrate_this_turn) return;
        _integrate_this_turn = true;
        
        //Apply linear body motion commands
        if (translate_body_cmnd == gc::LinearDirections::FORWARD) _translate_body_frc_cmnd += gc::TANK_BODY_FRWD_FRC_INC_CMND;
        else if (translate_body_cmnd == gc::LinearDirections::REVERSE) _translate_body_frc_cmnd -= gc::TANK_BODY_REV_FRC_DEC_CMND;

        //Apply rotational body motion commands
        if (rotate_body_cmnd == gc::AngularDirections::CCW) _rotate_body_torque_cmnd += gc::TANK_BODY_ROT_TRQ_INC_CMD;
        else if (rotate_body_cmnd == gc::AngularDirections::CW) _rotate_body_torque_cmnd -= gc::TANK_BODY_ROT_TRQ_INC_CMD;

        //Integrate dynamics to determine new pose of tank

        //Determine new position of turret (check for angular limits)

}

}