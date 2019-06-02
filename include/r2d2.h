#ifndef ZERKOLA_INCLUDE_R2D2_H_
#define ZERKOLA_INCLUDE_R2D2_H_

#include <tank.h>
#include <graphics.h>
#include <game_control.h>

namespace r2d2 {

/* Easy computer player */
class R2D2 : public tank::Tank {

/*=== START PRIVATE ===*/
private:
    /* Enemy target */
    tank::Tank* _enemyTarget;

    /* R2D2 turn. Policy based strategy */
    void _turn();

    /* Returns TRUE if tank is in danger of a missile strike */
    bool _dangerCheck() const;

    /* Moves the tank away from danger */
    void _evasiveManeuver();

    /* Orients the tank towrads _enemyTarget in order to fire */
    void _aggressiveManeuver();

    /* Returns TRUE if any missile would collide with tank on current trajectory */
    bool _directMissileHitCheck() const;
/*=== END PRIVATE ===*/

/*=== START PUBLIC ===*/
public:
    R2D2();
    ~R2D2();
    R2D2(graphics::Graphics* graphics_ptr, gc::PlayerColor player_color, std::list<missile::Missile*>* missiles_ptr);

    /* Set the enemy target */
    inline void setTarget(tank::Tank* enemy_target) { _enemyTarget = enemy_target; };
/*=== END PUBLIC ===*/

}; //class R2D2

} //namespace r2d2

#endif //ZERKOLA_INCLUDE_R2D2_H_