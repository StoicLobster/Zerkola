#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_

#include <player.h>
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

    player::Player _player_red; //TODO: Should be tank class and should be one for _player_blue
};

} //zerkola

#endif //ZERKOLA_INCLUDE_ZERKOLA_H_