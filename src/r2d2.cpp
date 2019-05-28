#include <r2d2.h>

namespace r2d2 {

R2D2::R2D2() {};

R2D2::~R2D2() {};

R2D2::R2D2(graphics::Graphics* graphics_ptr, gc::PlayerColor player_color, std::list<missile::Missile*>* missiles_ptr):
    tank::Tank(graphics_ptr, player_color, missiles_ptr)
    {};

void R2D2::_turn(double dt_ms) {};

}