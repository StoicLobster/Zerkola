#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_

#include <tank.h>
#include <missile.h>
#include <graphics.h>
#include <list>
#include <r2d2.h>

//#define DEBUG_ZERKOLA

namespace zerkola {

class Zerkola {
private:
    graphics::Graphics _graphics; //Must be first, this is where SDL_Init lives
    input::Input _input;
    SDL_Event _event; //Events that occur will be stored here by SDL    
    tank::Tank* _tank_red; //red tank
    tank::Tank* _tank_blue; //blue tank
    std::list<missile::Missile*> _missiles; //Missiles active in the game
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