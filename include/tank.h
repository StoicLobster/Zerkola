#ifndef ZERKOLA_INCLUDE_TANK_H_
#define ZERKOLA_INCLUDE_TANK_H_

#include <animated_sprite.h>
#include <input.h>
#include <game_control.h>
#include <Eigen/Dense>

namespace tank {

class Tank : public animated_sprite::AnimatedSprite {
protected:
Eigen::Vector2d _center, _dir, _v, _a; //Center, direction, and kinematic of tank
double _rad_collision; //radius of collision
Eigen::Rotation2Dd _rot_speed_cmd; //ro
bool _fire_this_turn, _accel_this_turn, _rotate_this_turn; //controls if each of these actions was taken this turn



public:
Tank();
Tank(gc::PlayerType player_type);

};

}

#endif //ZERKOLA_INCLUDE_TANK_H_