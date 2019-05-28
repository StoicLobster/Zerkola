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

/* Missile class. Contains logic for ricochet */
class Missile : public animated_sprite::AnimatedSprite {

/*=== START PRIVATE ===*/
private:
    /* Unique ID for missile */
    const unsigned int _ID;

    /* Center of missile */
    Eigen::Vector2d _center;

    /* Direction of missile */
    Eigen::Vector2d _dir;

    /* [m] Cumulative distance travelled by missile */
    long double _travel_dist;

    /* True if missile is active and capable of inducing a collision */
    bool _collision_active;

    /* Moves missile for a given frame. If missile would collide with boundary, will appropriately calculate ricochet dynamics */
    void _move(double dt_ms);

    /* Translates object in the current direction by given distance */
    void _translate(double dist);

    /* Rotates the missile to align with the given direction */
    void _rotate_align(const Eigen::Vector2d& dir_align);

    /* Sets pose in base Sprite class and transforms to screen coordinates (negate y) */
    void _setPose();

    /* Logic that happens when animation ends */
    void _animationDone(std::string currentAnimation);

    /* Sets up all animations for a sprite */
    void _setupAnimations();
/*=== END PRIVATE ===*/

/*=== START PUBLIC ===*/
public:
    Missile();
    virtual ~Missile();
    Missile(graphics::Graphics* graphics_ptr, int id, double x, double y, const Eigen::Vector2d& tank_dir);

    /* Moves missile and does housekeeping */
    void updateMissile(double dt_ms);

    /* Returns ID */
    inline int ID() const { return(_ID); };
/*=== END PUBLIC ===*/

}; //class Missile

} //namespace missile

#endif // ZERKOLA_INCLUDE_MISSILE_H