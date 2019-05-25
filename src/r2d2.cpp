#include <r2d2.h>

namespace r2d2 {

R2D2::R2D2() {};

R2D2::~R2D2() {};

R2D2::R2D2(graphics::Graphics& graphics, gc::PlayerColor player_color, std::list<missile::Missile*>* missiles_ptr):
    tank::Tank(graphics, player_color, missiles_ptr)
    {};

void R2D2::_turn(const double dt_ms) {};

}