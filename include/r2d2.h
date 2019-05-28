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
    /* R2D2 turn. Simple strategy */
    void _turn(double dt_ms);
/*=== END PRIVATE ===*/

/*=== START PUBLIC ===*/
public:
    R2D2();
    ~R2D2();
    R2D2(graphics::Graphics* graphics_ptr, gc::PlayerColor player_color, std::list<missile::Missile*>* missiles_ptr);
/*=== END PUBLIC ===*/

}; //class R2D2

} //namespace r2d2

#endif //ZERKOLA_INCLUDE_R2D2_H_