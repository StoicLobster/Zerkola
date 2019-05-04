#include <tank.h>
#include <algorithm>
#include <assert.h>

namespace tank {

Tank::Tank() {};

Tank::~Tank() {};

Tank::Tank(graphics::Graphics& graphics, gc::PlayerColor player_color): 
    animated_sprite::AnimatedSprite(
        graphics, 
        gc::TANK_SPRITE_ANIMATION_LOCATION, 
        player_color == gc::PlayerColor::RED ? gc::RED_TANK_SPRITE_START_X : gc::BLUE_TANK_SPRITE_START_X, 
        player_color == gc::PlayerColor::RED ? gc::RED_TANK_SPRITE_START_Y : gc::BLUE_TANK_SPRITE_START_Y, 
        gc::TANK_SPRITE_WIDTH, 
        gc::TANK_SPRITE_HEIGHT,
        player_color == gc::PlayerColor::RED ? gc::RED_PLAYER_START_POS_X : gc::BLUE_PLAYER_START_POS_X, 
        player_color == gc::PlayerColor::RED ? gc::RED_PLAYER_START_POS_Y : gc::BLUE_PLAYER_START_POS_Y, 
        gc::TANK_SPRITE_UPDATE_RATE_MS),
        _color(player_color),
        _input_ptr(nullptr),
        _l_body(0,1,0),
        _t_body(-1,0,0),
        _l_turret(0,1,0),
        _t_turret(-1,0,0),
        _body_lin_v(0,0,0),
        _body_lin_a(0,0,0),
        _body_ang_v(0,0,0),
        _body_ang_a(0,0,0),   
        _body_lin_v_prev(0,0,0),
        _body_lin_a_prev(0,0,0),
        _body_ang_v_prev(0,0,0),
        _body_ang_a_prev(0,0,0),   
        _rotate_body_torque_cmnd(0),
        _translate_body_frc_cmnd(0),
        _rotate_turret_spd_cmnd(0),
        _tractive_accel_limit_mag(0),
        _slip(false),
        _num_missiles(gc::MAX_MISSILES_PER_PLAYER),
        _fire_this_turn(false),
        _move_this_turn(false)
{
    //Set starting position in game
    int start_x, start_y;
    if (_color == gc::PlayerColor::RED) {
        start_x = gc::RED_PLAYER_START_POS_X;
        start_y = gc::RED_PLAYER_START_POS_Y;
    } else {
        start_x = gc::BLUE_PLAYER_START_POS_X;
        start_y = gc::BLUE_PLAYER_START_POS_Y;
    }
    _body_center.x = start_x;
    _body_center.y = start_y;
    _body_center.z = 0;
    _turrent_center.x = start_x;
    _turrent_center.y = start_y;
    _turrent_center.z = 0;

    //Setup animations
    setupAnimations();
};

void Tank::animationDone(std::string currentAnimation) {};

void Tank::setupAnimations() {
    int sprite_start_x, sprite_start_y;
    if (_color == gc::PlayerColor::RED) {
        sprite_start_x = gc::RED_TANK_SPRITE_START_X;
        sprite_start_y = gc::RED_TANK_SPRITE_START_Y;
    } else {
        sprite_start_x = gc::BLUE_TANK_SPRITE_START_X;
        sprite_start_y = gc::BLUE_TANK_SPRITE_START_Y;
    }
    addAnimation(gc::NUMBER_TANK_SPRITE_ROLL_ANIMATIONS, sprite_start_x, sprite_start_y, "RollForward", gc::TANK_SPRITE_WIDTH, gc::TANK_SPRITE_HEIGHT, Eigen::Vector2d(0,0), false);
    addAnimation(gc::NUMBER_TANK_SPRITE_ROLL_ANIMATIONS, sprite_start_x, sprite_start_y, "RollBackward", gc::TANK_SPRITE_WIDTH, gc::TANK_SPRITE_HEIGHT, Eigen::Vector2d(0,0), true);
    return;
}

void Tank::turn(const double elapsed_time) {
    gc::LinearDirections translate_body_cmnd = gc::LinearDirections::NONE;
    gc::AngularDirections rotate_body_cmnd = gc::AngularDirections::NONE;
    gc::AngularDirections rotate_turret_cmnd = gc::AngularDirections::NONE;
    //Parse input and execute move / fire
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_SPACE) || _input_ptr->isKeyHeld(SDL_SCANCODE_SPACE)) {
        //Fire missile
        _fire(); //TODO Pass in missile list from Zerkola
    }
    //TODO finish possible input commands
    //arrow keys for movement
    //a/d for rotate turret

    
    return;
}

void Tank::_move(const double dt, 
    const gc::LinearDirections translate_body_cmnd, 
    const gc::AngularDirections rotate_body_cmnd,
    const gc::AngularDirections rotate_turret_cmnd) {
    //Check if move already occurred
    if (_move_this_turn) return;
    _move_this_turn = true;

    //Check unit vectors
    assert(_l_body.norm() == 1.0);
    assert(_t_body.norm() == 1.0);
    assert(_l_turret.norm() == 1.0);
    assert(_t_turret.norm() == 1.0);

    //Store previous states
    _body_lin_v_prev = _body_lin_v;
    _body_lin_a_prev = _body_lin_a;
    _body_ang_v_prev = _body_ang_v;
    _body_ang_a_prev = _body_ang_a;
    
    //Apply linear body motion commands
    if (translate_body_cmnd == gc::LinearDirections::FORWARD) _translate_body_frc_cmnd += gc::TANK_BODY_FRWD_FRC_INC_CMND;
    else if (translate_body_cmnd == gc::LinearDirections::REVERSE) _translate_body_frc_cmnd -= gc::TANK_BODY_REV_FRC_DEC_CMND;
    //Limit force command
    _translate_body_frc_cmnd = std::max( _translate_body_frc_cmnd, gc::TANK_BODY_MAX_REV_FRC ); //tank force limits
    _translate_body_frc_cmnd = std::min( _translate_body_frc_cmnd, gc::TANK_BODY_MAX_FRWD_FRC );

    //Apply rotational body motion commands
    if (rotate_body_cmnd == gc::AngularDirections::CCW) _rotate_body_torque_cmnd += gc::TANK_BODY_ROT_TRQ_INC_CMD;
    else if (rotate_body_cmnd == gc::AngularDirections::CW) _rotate_body_torque_cmnd -= gc::TANK_BODY_ROT_TRQ_INC_CMD;
    //Limit torque command
    _rotate_body_torque_cmnd = std::max( _rotate_body_torque_cmnd, -1*gc::TANK_BODY_MAX_ROT_TRQ ); //tank torque limits
    _rotate_body_torque_cmnd = std::min( _rotate_body_torque_cmnd, gc::TANK_BODY_MAX_ROT_TRQ );

    //Determine if slip is occurring
    _slip = _body_lin_a.norm() > _tractive_accel_limit_mag;

    //Arbitrate slip dynamics
    if (!_slip) {
        _tractive_accel_limit_mag = gc::g*gc::SURF_STATIC_MU;

        /*** Newton's Second Law - Moment Balance ***/
        double tau = _rotate_body_torque_cmnd;
        _body_ang_a = tau/gc::TANK_MOMENT_OF_INERTIA_Z * gc::Z;
        //Integrate angular acceleration
        _integrate(dt, _body_ang_v, _body_ang_a_prev, _body_ang_a);
        //Integrate angular velocity
        double theta_delta = 0; //TODO: Cleaner way to handle angular position?
        _integrate(dt, theta_delta, _body_ang_v_prev.norm(), _body_ang_v.norm());
        Eigen::Rotation2Dd rot(theta_delta);
        _l_body = rot*_l_body;
        _t_body = rot*_t_body;
        _l_turret = rot*_l_turret;
        _t_turret = rot*_t_turret;
        _l_body.normalize();
        _t_body.normalize();
        _l_turret.normalize();
        _t_turret.normalize();
        /*** Newton's Second Law - Force Balance ***/
        Eigen::Vector3d F_RR, F_prop, F_road;
        F_RR = gc::TANK_ROLLING_RESIST_FRC * -1 * _l_body; //rolling resistance of tracks (resists longitudinal motion)
        F_prop = _translate_body_frc_cmnd * _l_body; //total propulsion force
        F_road = _body_ang_v.cross(_body_lin_v); //centripetal force applied by road (since no slip we know road can supply it)
        _body_lin_a = 1/gc::TANK_MASS * (F_RR + F_prop + F_road);
        //Integrate linear acceleration
        _integrate(dt, _body_lin_v, _body_lin_a_prev, _body_lin_a);
        //Integrate linear velocity
        _integrate(dt, _body_center, _body_lin_v_prev, _body_lin_v);
        _integrate(dt, _turrent_center, _body_lin_v_prev, _body_lin_v);
        
    } else if (!_slip) {
        _tractive_accel_limit_mag = gc::g*gc::SURF_KINETIC_MU;

        //Really no idea how to model slip. This is a total guess.
        /*** Newton's Second Law - Force Balance ***/
        //Only force is force of kinetic friction in a direction opposite to current velocity
        Eigen::Vector3d F_fric = gc::TANK_MASS * gc::g * gc::SURF_KINETIC_MU * -1 * _body_lin_v / _body_lin_v.norm();
        _body_lin_a = 1/gc::TANK_MASS * F_fric;
        //Integrate linear acceleration
        _integrate(dt, _body_lin_v, _body_lin_a_prev, _body_lin_a);
        //Integrate linear velocity
        _integrate(dt, _body_center, _body_lin_v_prev, _body_lin_v);
        _integrate(dt, _turrent_center, _body_lin_v_prev, _body_lin_v);
    }

    /*** Rotate Tank Turret ***/
    if (rotate_turret_cmnd == gc::AngularDirections::CCW) _rotate_turret_spd_cmnd = gc::TANK_TURRET_ROT_SPD;
    else if (rotate_turret_cmnd == gc::AngularDirections::CW) _rotate_turret_spd_cmnd = -1 * gc::TANK_TURRET_ROT_SPD;
    double theta_delta = _rotate_turret_spd_cmnd*dt;
    Eigen::Rotation2Dd rot(theta_delta);
    _l_turret = rot*_l_turret;
    _t_turret = rot*_t_turret;
    _l_turret.normalize();
    _t_turret.normalize();
    
    return;
}

void Tank::_fire(std::list<missile::Missile*>& missiles) {
    if (_fire_this_turn) return;
    _fire_this_turn = true;
    if (_num_missiles <= 0) return;
    missile::Missile* missile_ptr = new missile::Missile(num_missiles, _turrent_center.x(), _turrent_center.y(), _l_turret);
    missiles.push_back(missile_ptr);
    --_num_missiles;
    return;
}

}