#include <tank.h>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <geometry.h>

namespace tank {

Tank::Tank() {};

Tank::~Tank() {};

Tank::Tank(graphics::Graphics* graphics_ptr, 
    gc::PlayerColor player_color, 
    std::list<missile::Missile*>* missiles_ptr): 

animated_sprite::AnimatedSprite(
    graphics_ptr, 
    gc::SPRITE_ANIMATION_FILE_PATH, 
    player_color == gc::PlayerColor::RED ? gc::RED_TANK_BODY_SPRITE_START_X : gc::BLUE_TANK_BODY_SPRITE_START_X, 
    player_color == gc::PlayerColor::RED ? gc::RED_TANK_BODY_SPRITE_START_Y : gc::BLUE_TANK_BODY_SPRITE_START_Y, 
    gc::TANK_BODY_SPRITE_WIDTH, 
    gc::TANK_BODY_SPRITE_HEIGHT,
    gc::TANK_BODY_SPRITE_UPDATE_RATE_MS),

_color(player_color),
_graphics_ptr(graphics_ptr),

_turret(graphics_ptr, 
    gc::SPRITE_ANIMATION_FILE_PATH, 
    player_color == gc::PlayerColor::RED ? gc::RED_TANK_TURRET_SPRITE_START_X : gc::BLUE_TANK_TURRET_SPRITE_START_X, 
    player_color == gc::PlayerColor::RED ? gc::RED_TANK_TURRET_SPRITE_START_Y : gc::BLUE_TANK_TURRET_SPRITE_START_Y, 
    gc::TANK_TURRET_SPRITE_WIDTH, 
    gc::TANK_TURRET_SPRITE_HEIGHT),

_l_body(gc::Y_3D_PHYS.cast<double>()),
_t_body(-1*gc::X_3D_PHYS.cast<double>()),
_l_turret(gc::Y_3D_PHYS.cast<double>()),
_t_turret(-1*gc::X_3D_PHYS.cast<double>()),
_k(gc::Z_3D_PHYS.cast<double>()),
_body_lin_v(0.0,0.0,0.0),
_body_lin_a(0.0,0.0,0.0),
_body_ang_v(0.0,0.0,0.0),
_body_ang_a(0.0,0.0,0.0),   
_body_lin_v_prev(0.0,0.0,0.0),
_body_lin_a_prev(0.0,0.0,0.0),
_body_ang_v_prev(0.0,0.0,0.0),
_body_ang_a_prev(0.0,0.0,0.0),   
_rotate_body_torque_cmnd(0.0),
_translate_body_frc_cmnd(0.0),
_rotate_turret_spd_cmnd(0.0),
_tractive_accel_limit_mag(0.0),
_slip(false),
_ammo(gc::MAX_MISSILES_PER_PLAYER),
_fire_this_turn(false),
_move_this_turn(false),
_motionState(gc::TankMotionState::STATIONARY),
_shiftTimer(0.0),
_printTimer_ms(0.0),
_printInterval_ms(100.0),
_enemyTarget(nullptr),
_missiles_ptr(missiles_ptr),
_translate_body_cmnd(gc::LinearDirection::LINEAR_DIRECTION_NONE),
_rotate_body_cmnd(gc::AngularDirection::ANGULAR_DIRECTION_NONE),
_rotate_turret_cmnd(gc::AngularDirection::ANGULAR_DIRECTION_NONE)
{
    //Set starting position in game
    double start_x, start_y;
    if (_color == gc::PlayerColor::RED) {
        start_x = gc::RED_PLAYER_START_POS_X_PHYS;
        start_y = gc::RED_PLAYER_START_POS_Y_PHYS;
    } else {
        start_x = gc::BLUE_PLAYER_START_POS_X_PHYS;
        start_y = gc::BLUE_PLAYER_START_POS_Y_PHYS;
    }
    //Set starting vectors
    _center.x() = start_x;
    _center.y() = start_y;
    _center.z() = 0.0;

    //Setup animations
    _setupAnimations();
};

void Tank::_animationDone(std::string currentAnimation) {}; //TODO?

void Tank::_setupAnimations() {
    int sprite_start_x, sprite_start_y;
    if (_color == gc::PlayerColor::RED) {
        sprite_start_x = gc::RED_TANK_BODY_SPRITE_START_X;
        sprite_start_y = gc::RED_TANK_BODY_SPRITE_START_Y;
    } else {
        sprite_start_x = gc::BLUE_TANK_BODY_SPRITE_START_X;
        sprite_start_y = gc::BLUE_TANK_BODY_SPRITE_START_Y;
    }
    _addAnimation(1, sprite_start_x, sprite_start_y, "Idle", gc::TANK_BODY_SPRITE_WIDTH, gc::TANK_BODY_SPRITE_HEIGHT, false);
    _addAnimation(gc::TANK_BODY_NUMBER_SPRITE_ROLL_ANIMATIONS, sprite_start_x, sprite_start_y, "RollForward", gc::TANK_BODY_SPRITE_WIDTH, gc::TANK_BODY_SPRITE_HEIGHT, false);
    _addAnimation(gc::TANK_BODY_NUMBER_SPRITE_ROLL_ANIMATIONS, sprite_start_x, sprite_start_y, "RollBackward", gc::TANK_BODY_SPRITE_WIDTH, gc::TANK_BODY_SPRITE_HEIGHT, true);
    //TODO add turning animations
    return;
}

void Tank::_setPose() {
    //Tank Body
    Eigen::Vector2d d_body(_l_body.x(), _l_body.y());
    this->setDirection(d_body);
    Eigen::Vector2i c_body(gc::TANK_BODY_CENTER_RELATIVE_TO_UL_X, gc::TANK_BODY_CENTER_RELATIVE_TO_UL_Y);
    this->setCOR(c_body);
    int center_x = static_cast<int>(std::round(_center.x()));
    int center_y = static_cast<int>(std::round(-1*_center.y()));
    int body_UL_x = center_x - gc::TANK_BODY_CENTER_RELATIVE_TO_UL_X;    
    int body_UL_y = center_y - gc::TANK_BODY_CENTER_RELATIVE_TO_UL_Y;
    assert(body_UL_x >= 0);
    assert(body_UL_y >= 0);
    Eigen::Vector2i UL_body(body_UL_x, body_UL_y);
    this->setUL(UL_body);
    //Tank Turret
    Eigen::Vector2d d_turret(_l_turret.x(), _l_turret.y());
    _turret.setDirection(d_turret);
    int turret_UL_x = center_x - gc::TANK_TURRET_CENTER_RELATIVE_TO_UL_X;    
    int turret_UL_y = center_y - gc::TANK_TURRET_CENTER_RELATIVE_TO_UL_Y;
    assert(turret_UL_x >= 0);
    assert(turret_UL_y >= 0);
     Eigen::Vector2i UL_turret(turret_UL_x, turret_UL_y);
    _turret.setUL(UL_turret);
    Eigen::Vector2i c_turret(gc::TANK_TURRET_CENTER_RELATIVE_TO_UL_X, gc::TANK_TURRET_CENTER_RELATIVE_TO_UL_Y);
    _turret.setCOR(c_turret);

    return;
}

void Tank::_determineMotionState(double dt_s) {
    if (_motionState == gc::TankMotionState::STATIONARY && 
        _translate_body_cmnd == gc::LinearDirection::BACKWARD) {
        //Shifting Start
        _shiftTimer = 0.0;
        _motionState = gc::TankMotionState::SHIFTING;

    } else if (_motionState == gc::TankMotionState::STATIONARY && 
               _translate_body_cmnd == gc::LinearDirection::FORWARD   ) {
        _motionState = gc::TankMotionState::PROPULSION_FORWARD;

    } else if (_motionState == gc::TankMotionState::SHIFTING) {
        //Shifting In Progress
        _shiftTimer += dt_s;
        if (_shiftTimer >= gc::TANK_SHIFT_TIME) {
            if (_translate_body_cmnd == gc::LinearDirection::FORWARD) _motionState = gc::TankMotionState::PROPULSION_FORWARD;
            else if (_translate_body_cmnd == gc::LinearDirection::BACKWARD) _motionState = gc::TankMotionState::REVERSE;
        }
    } else if (_motionState == gc::TankMotionState::PROPULSION_FORWARD) {
        if (_translate_body_cmnd == gc::LinearDirection::BACKWARD) _motionState = gc::TankMotionState::BRAKING;

    } else if (_body_lin_v.x() == 0.0) {
        //Stationary
        _motionState = gc::TankMotionState::STATIONARY;

    }
    //Determine Animation
    if (_motionState == gc::TankMotionState::PROPULSION_FORWARD || _motionState == gc::TankMotionState::BRAKING) playAnimation("RollForward");
    else if (_motionState == gc::TankMotionState::REVERSE) playAnimation("RollBackward");
    else playAnimation("Idle");

    return;
}

void Tank::_move(double dt_ms) {
    //Check if move already occurred
    if (_move_this_turn) return;
    _move_this_turn = true;

    if (dt_ms == 0.0) return;
    double dt_s = dt_ms/1000.0;

    //Normalize unit vectors
    _l_body.normalize();
    _t_body.normalize();
    _l_turret.normalize();
    _t_turret.normalize();

    //Store previous states
    _body_lin_v_prev = _body_lin_v;
    _body_lin_a_prev = _body_lin_a;
    _body_ang_v_prev = _body_ang_v;
    _body_ang_a_prev = _body_ang_a;
    
    //Arbitrate motion state
    _determineMotionState(dt_s);

    //Determine translational force commands
    if (_motionState == gc::TankMotionState::PROPULSION_FORWARD) {
        //Increase propulsion torque up to max (rate limited)
        double tmp_cmnd = gc::TANK_BODY_MAX_FRWD_FRC - _translate_body_frc_cmnd;
        _translate_body_frc_cmnd += std::min(gc::TANK_BODY_FRWD_FRC_RATE_LIMIT*dt_s, tmp_cmnd);
    } else if (_motionState == gc::TankMotionState::REVERSE) {
        if (_translate_body_cmnd == gc::LinearDirection::BACKWARD) {
            //Increase propulsion torque up to max (rate limited)
            double tmp_cmnd = gc::TANK_BODY_MAX_REV_FRC - _translate_body_frc_cmnd;
            _translate_body_frc_cmnd += std::min(gc::TANK_BODY_FRWD_FRC_RATE_LIMIT*dt_s, tmp_cmnd);
        } else if (_translate_body_cmnd == gc::LinearDirection::FORWARD) {
            //Increase propulsion torque up to max (rate limited)
            double tmp_cmnd = gc::TANK_BODY_MAX_FRWD_FRC - _translate_body_frc_cmnd;
            _translate_body_frc_cmnd += std::min(gc::TANK_BODY_FRWD_FRC_RATE_LIMIT*dt_s, tmp_cmnd);
        }
    } else if (_motionState == gc::TankMotionState::BRAKING) {
        _translate_body_frc_cmnd = gc::TANK_BODY_BRK_FRC;
    } else if (_motionState == gc::TankMotionState::STATIONARY || _motionState == gc::TankMotionState::SHIFTING) {
        _translate_body_frc_cmnd = 0.0;
    }

    //Apply rotational body motion commands
    _rotate_body_torque_cmnd = 0;
    if (_rotate_body_cmnd == gc::AngularDirection::CCW) {
        _rotate_body_torque_cmnd = gc::TANK_BODY_ROT_TRQ_CMND;
    } else if (_rotate_body_cmnd == gc::AngularDirection::CW) {
        _rotate_body_torque_cmnd = -1*gc::TANK_BODY_ROT_TRQ_CMND;
    }

    //Determine if slip is occurring
    //_slip = _body_lin_a.norm() > _tractive_accel_limit_mag;
    _slip = false; //TODO: Calibrate slip

    //Arbitrate slip dynamics and model motion
    if (!_slip) {
        /* NO SLIP */
        _tractive_accel_limit_mag = gc::g*gc::SURF_STATIC_MU;

        /*** Newton's Second Law - Moment Balance ***/
        double M_RR = 0;
        if (_body_ang_v.z() > 0) M_RR = -1*gc::RR_TRQ;
        else if (_body_ang_v.z() < 0) M_RR = gc::RR_TRQ;
        _body_ang_a = (_rotate_body_torque_cmnd + M_RR)/gc::TANK_MOMENT_OF_INERTIA_Z * gc::Z_3D_PHYS.cast<double>();
        // Integrate angular acceleration
        _integrate(dt_s, _body_ang_v, _body_ang_a_prev, _body_ang_a);
        // Integrate angular velocity
        double theta_delta = 0.0; //TODO: Cleaner way to handle angular position?
        _integrate(dt_s, theta_delta, _body_ang_v_prev.z(), _body_ang_v.z());
        if (theta_delta != 0.0) {
            Eigen::AngleAxis<double> rot(theta_delta, _k);
            _l_body = rot*_l_body;
            _t_body = rot*_t_body;
            _l_turret = rot*_l_turret;
            _t_turret = rot*_t_turret;
            _l_body.normalize();
            _t_body.normalize();
            _l_turret.normalize();
            _t_turret.normalize();
        }

        /*** Newton's Second Law - Force Balance ***/
        Eigen::Vector3d F_prop(0,0,0), F_road(0,0,0), F_RR(0,0,0);
        F_prop = _translate_body_frc_cmnd * _l_body; // total propulsion force
        F_road = _body_ang_v.cross(_body_lin_v)*gc::TANK_MASS; // centripetal force applied by road (since no slip we know road can supply it)
        if (_body_lin_v.norm() !=  0.0) F_RR = -1*gc::RR_FRC*_body_lin_v.normalized();
        _body_lin_a = 1/gc::TANK_MASS * (F_prop + F_road + F_RR);
        // Integrate linear acceleration
        _integrate(dt_s, _body_lin_v, _body_lin_a_prev, _body_lin_a);
        // Check for velocity sign chage
        if ((_body_lin_v.x() < 0.0 && _body_lin_v_prev.x() > 0.0) ||
            (_body_lin_v.x() > 0.0 && _body_lin_v_prev.x() < 0.0)) {
            //Enforce pure zero velocity
            _body_lin_v.x() = 0.0;  
        }
        // Enforce velocity limits
        double v_lin_mag = _body_lin_v.norm();
        v_lin_mag = std::max(v_lin_mag, gc::TANK_BODY_MIN_LONG_VEL);
        v_lin_mag = std::min(v_lin_mag, gc::TANK_BODY_MAX_LONG_VEL);
        _body_lin_v = v_lin_mag*_body_lin_v.normalized();
        // Integrate linear velocity
        _integrate(dt_s, _center, _body_lin_v_prev, _body_lin_v);
        
    } else {
        /* SLIP */
        _tractive_accel_limit_mag = gc::g*gc::SURF_KINETIC_MU;

        // Really no idea how to model slip. This is a total guess.
        /*** Newton's Second Law - Force Balance ***/
        // Only force is force of kinetic friction in a direction opposite to current velocity
        Eigen::Vector3d v_dir = _body_lin_v / _body_lin_v.norm();
        Eigen::Vector3d F_fric = gc::TANK_MASS * gc::g * gc::SURF_KINETIC_MU * -1 * v_dir;
        _body_lin_a = 1/gc::TANK_MASS * F_fric;
        // Integrate linear acceleration
        _integrate(dt_s, _body_lin_v, _body_lin_a_prev, _body_lin_a);
        // Integrate linear velocity
        _integrate(dt_s, _center, _body_lin_v_prev, _body_lin_v);
    }

    // Limit Tank Position
    if (_center.x() < gc::BOARD_PHYS_LEFT) _center.x() = gc::BOARD_PHYS_LEFT;
    if (_center.x() > gc::BOARD_PHYS_RIGHT) _center.x() = gc::BOARD_PHYS_RIGHT;
    if (_center.y() < gc::BOARD_PHYS_BOTTOM) _center.y() = gc::BOARD_PHYS_BOTTOM;
    if (_center.y() > gc::BOARD_PHYS_TOP) _center.y() = gc::BOARD_PHYS_TOP;

    /*** Rotate Tank Turret ***/
    _rotate_turret_spd_cmnd = 0;
    if (_rotate_turret_cmnd == gc::AngularDirection::CCW) _rotate_turret_spd_cmnd = gc::TANK_TURRET_ROT_SPD*dt_s;
    else if (_rotate_turret_cmnd == gc::AngularDirection::CW) _rotate_turret_spd_cmnd = -1 * gc::TANK_TURRET_ROT_SPD*dt_s;

    //Check if new turret rotation would exceed limits 
    double alpha = geo::AngBetweenVecs(_l_body, _l_turret)*geo::RAD_TO_DEG;
    if ( ((alpha + _rotate_turret_spd_cmnd) < gc::TANK_TURRET_MAX_ANG) && ((alpha + _rotate_turret_spd_cmnd) > -1*gc::TANK_TURRET_MAX_ANG) ) {
        //Rotate
        double theta_delta_max = gc::TANK_TURRET_MAX_ANG - alpha;
        double theta_delta_min = -1*gc::TANK_TURRET_MAX_ANG - alpha;
        double theta_delta = _rotate_turret_spd_cmnd;
        theta_delta = std::max(theta_delta, theta_delta_min);
        theta_delta = std::min(theta_delta, theta_delta_max);
        Eigen::AngleAxis<double> rot(theta_delta/geo::RAD_TO_DEG, _k);
        _l_turret = rot*_l_turret;
        _t_turret = rot*_t_turret;
        _l_turret.normalize();
        _t_turret.normalize();
    }
    
    return;
}

void Tank::_fire() {
    if (_fire_this_turn) return;
    _fire_this_turn = true;
    if (_ammo <= 0) return;
    // Instantiate new missile
    short new_ID = gc::MAX_MISSILES_PER_PLAYER - _ammo;
    Eigen::Vector2d tank_dir(_l_turret.x(),_l_turret.y());
    missile::Missile* missile_ptr = new missile::Missile(_graphics_ptr, new_ID, _center.x(), _center.y(), tank_dir);
    // Add to missile list
    _missiles_ptr->push_back(missile_ptr);
    // Reduce ammo
    --_ammo;
    return;
}

void Tank::drawTank() {
    //Draw body
    animated_sprite::AnimatedSprite::draw();
    //Draw turret
    _turret.draw();
    return;
}

void Tank::_resetTurn() {
    _fire_this_turn = false; 
    _move_this_turn = false; 
    _translate_body_cmnd = gc::LinearDirection::LINEAR_DIRECTION_NONE;
    _rotate_body_cmnd = gc::AngularDirection::ANGULAR_DIRECTION_NONE;
    _rotate_turret_cmnd = gc::AngularDirection::ANGULAR_DIRECTION_NONE;
    return;
}

bool Tank::update(double dt_ms, bool verbose) {
    // Ensure enemy has been set
    assert(_enemyTarget);
    //Update animatedSprite
    animated_sprite::AnimatedSprite::update(dt_ms);
    //Reset turn
    _resetTurn();
    //Take turn
    _turn();
    //Execute move command
    _move(dt_ms);
    //Set pose in base class
    _setPose();
    if (verbose) {
        _printTimer_ms += dt_ms;
        if (_printTimer_ms >= _printInterval_ms) {
            _printTimer_ms = 0.0;
            std::cout << std::endl << "===== START TANK STATE =====" << std::endl << std::endl;
            _printState();
            std::cout << "===== END TANK STATE =====" << std::endl << std::endl;
        }
    }
    return(_collisionCheck());
}

bool Tank::_collisionCheck() const {
    Eigen::Vector2d tank_center(_center.x(),_center.y());
    for (auto missile : (*_missiles_ptr)) {
        double dist = (tank_center - missile->center()).norm();
        if (missile->collision_active() && (dist <= (gc::TANK_RAD_COL + gc::MISSILE_RAD_COL))) return(true);
    }
    return(false);
}

void Tank::_printState() const {
    std::cout << "Basic State" << std::endl;
    std::cout << "\tTank Color:" << _color << std::endl;
    std::cout << "\tAmmo: " << _ammo << std::endl;
    std::cout << "\tFired This Turn: " << _fire_this_turn << std::endl;
    std::cout << "\tMoved This Turn: " << _move_this_turn << std::endl;

    std::cout << "Overall Motion State" << std::endl;
    std::cout << "\tMotion State: " << _motionState << std::endl;
    std::cout << "\tShift Timer [ms]: " << _shiftTimer << std::endl;
    std::cout << "\tSlip: ";
    if (_slip) std::cout << "TRUE" << std::endl;
    else std::cout << "FALSE" << std::endl;
    std::cout << "\tTraction Accel Limit: " << _tractive_accel_limit_mag << std::endl;
    

    std::cout << "Linear State" << std::endl;
    std::cout << "\tPosition: (" << _center.x() << "," << _center.y() << ")" << std::endl;
    std::cout << "\tVelocity: (" << _body_lin_v.x() << "," << _body_lin_v.y() << ")" << std::endl;
    std::cout << "\tAcceleration: (" << _body_lin_a.x() << "," << _body_lin_a.y() << ")" << std::endl;

    std::cout << "Angular State" << std::endl;
    std::cout << "\tLongitudinal Direction: (" << _l_body.x() << "," << _l_body.y() << "," << _l_body.z() << ")" << std::endl;
    std::cout << "\tTransverse Direction: (" << _t_body.x() << "," << _t_body.y() << "," << _t_body.z() << ")" << std::endl;
    std::cout << "\tVelocity: (" << _body_ang_v.x() << "," << _body_ang_v.y() << "," << _body_ang_v.z() << ")" << std::endl;
    std::cout << "\tAcceleration: (" << _body_ang_a.x() << "," << _body_ang_a.y() << "," << _body_ang_a.z() << ")" << std::endl;

    std::cout << "Turret Direction" << std::endl;
    std::cout << "\tLongitudinal: (" << _l_turret.x() << "," << _l_turret.y() << "," << _l_turret.z() << ")" << std::endl;
    std::cout << "\tTransverse: (" << _t_turret.x() << "," << _t_turret.y() << "," << _t_turret.z() << ")" << std::endl;

    std::cout << "Motion Commands" << std::endl;
    std::cout << "\tTranslate Body Command: " << _translate_body_cmnd << std::endl;
    std::cout << "\tRotate Body Command: " << _rotate_body_cmnd << std::endl;
    std::cout << "\tRotate Turret Command: " << _rotate_turret_cmnd << std::endl;

    std::cout << "Control Commands" << std::endl;
    std::cout << "\tTranslate Body Force: " << _translate_body_frc_cmnd << std::endl;
    std::cout << "\tRotate Body Torque: " << _rotate_body_torque_cmnd << std::endl;
    std::cout << "\tRotate Turret Speed:" << _rotate_turret_spd_cmnd << std::endl;
    
    return;
}

}