#include <r2d2.h>

namespace r2d2 {

R2D2::R2D2() {};

R2D2::~R2D2() {};

R2D2::R2D2(graphics::Graphics* graphics_ptr, gc::PlayerColor player_color, std::list<missile::Missile*>* missiles_ptr):
    tank::Tank(graphics_ptr, player_color, missiles_ptr)

    _enemyTarget(null_ptr)
    {};

void R2D2::_turn(double dt_ms) {
    //If (tank in danger)
        //Evasive action
    //Else
        //Aggressive action
};

bool _directMissileHit() const {
    for (auto missile : (*_missiles_ptr)) {
        if LineCircleIntersection(missile->) {
            return(true);
        }
    }
    return(false);
}

}