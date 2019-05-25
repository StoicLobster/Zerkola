#ifndef ZERKOLA_INCLUDE_MISSILE_H
#define ZERKOLA_INCLUDE_MISSILE_H
//Includes
#include <animated_sprite.h>
#include <Eigen/Dense>
#include <game_control.h>
#include <list>
#include <geometry.h>

//#define DEBUG_RICOCHET

namespace missile {

class Missile : public animated_sprite::AnimatedSprite {

private:
//members
const unsigned int _ID; //unique ID for missile
Eigen::Vector2d _center; //Center of missile
Eigen::Vector2d _dir; //Direction of missile
long double _travel_dist; //cumulative distance travelled by missile
bool _collision_active;
//methods
void _translate(const double& dist); //Translates object in the current direction by given distance
void _rotate_align(const Eigen::Vector2d& dir_align); //rotates the missile to align with the given direction

/* Virtual Functions
 *
 */
void _animationDone(std::string currentAnimation);
void _setupAnimations();

public:
//constructors
Missile();
Missile(graphics::Graphics& graphics, const int& id, const double& x, const double& y, const Eigen::Vector2d& tank_dir);
virtual ~Missile();
//methods
void Move(); //Moves missile. If missile would collide with boundary, instead will calculate ricochet.
inline int ID() const { return(_ID); };

};

} //namespace missile

#endif // ZERKOLA_INCLUDE_MISSILE_H