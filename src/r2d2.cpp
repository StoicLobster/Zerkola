#include <r2d2.h>

namespace r2d2 {

R2D2::R2D2() {};

R2D2::~R2D2() {};

R2D2::R2D2(graphics::Graphics& graphics, gc::PlayerColor player_color):
    tank::Tank(graphics, player_color)
    {};

void R2D2::_turn(const double dt_ms) {};

}