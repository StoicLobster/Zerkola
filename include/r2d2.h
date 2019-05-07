#ifndef ZERKOLA_INCLUDE_R2D2_H_
#define ZERKOLA_INCLUDE_R2D2_H_

#include <tank.h>
#include <graphics.h>
#include <game_control.h>

namespace r2d2 {

class R2D2 : public tank::Tank {
private:

public:
R2D2();
~R2D2();
R2D2(graphics::Graphics& graphics, gc::PlayerColor player_color);

void _turn(const double dt_ms);

};

}

#endif