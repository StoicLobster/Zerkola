#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_

#include <animated_sprite.h>
#include <graphics.h>

namespace zerkola {

class Zerkola {
public:
    Zerkola();
    ~Zerkola();
private:
    void loop(); //core loop. Runs every frame.
    void draw(graphics::Graphics &graphics); //draws grpahics
    void update(double elaspedTime); //updates graphics

    animated_sprite::AnimatedSprite _player_A;
};

} //zerkola

#endif //ZERKOLA_INCLUDE_ZERKOLA_H_