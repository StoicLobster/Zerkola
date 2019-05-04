#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_

#include <tank.h>
#include <graphics.h>

namespace zerkola {

class Zerkola {
private:
    input::Input _input;
    SDL_Event _event; //Events that occur will be stored here by SDL
    graphics::Graphics _graphics;
    tank::Tank _tank_red; //red tank
    tank::Tank _tank_blue; //blue tank
    double _elapsedTime; //Elapsed time this frame

    void loop(); //core loop. Runs every frame.
    void draw(); //draws grpahics
    void update(); //updates graphics    

    public:

    Zerkola();
    ~Zerkola();
};

} //zerkola

#endif //ZERKOLA_INCLUDE_ZERKOLA_H_