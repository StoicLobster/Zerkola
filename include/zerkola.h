#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_

#include <tank.h>
#include <missile.h>
#include <graphics.h>
#include <list>
#include <r2d2.h>

//#define DEBUG_ZERKOLA

namespace zerkola {

/* Core game class */
class Zerkola {

/*=== START PRIVATE ===*/
private:
    /* Graphics for the game */
    graphics::Graphics _graphics; //

    /* Inputs for the human player */
    input::Input _input;

    /* Events that occur will be stored here by SDL */
    SDL_Event _event;    

    /* Red tank. This is always the computer player */
    tank::Tank* _tank_red;

    /* Blue tank. This is always the human player */
    tank::Tank* _tank_blue;

    /* Missiles active in the game */
    std::list<missile::Missile*> _missiles;

    /* Elapsed time this frame [ms] */
    double _elapsedTime;

    /* Setup game. Runs once at the start */
    bool setup();

    /* Core game loop. Runs every frame */
    void loop();

    /* Update game (physics) */
    void update();

    /* Draw graphics (render) */
    void draw();
/*=== END PRIVATE ===*/    

/*=== START PUBLIC ===*/
public:
    Zerkola();
    ~Zerkola();
/*=== END PUBLIC ===*/

}; //class Zerkola

} //namespace zerkola

#endif //ZERKOLA_INCLUDE_ZERKOLA_H_