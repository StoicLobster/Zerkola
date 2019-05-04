/* Zerkola class
 * Home for Zerkola game itself
 */

#include <zerkola.h>
#include <game_control.h>
#include <SDL2/SDL.h>
#include <input.h>
#include <algorithm>

namespace zerkola {

Zerkola::Zerkola() {
    SDL_Init(SDL_INIT_EVERYTHING);
    loop(); //begin game
}

Zerkola::~Zerkola() {}

void Zerkola::loop() {         
    //Instantiate players
    _tank_blue = tank::Tank(_graphics, gc::PlayerColor::BLUE, &_input); //Always human player
    //_tank_red = tank::Tank(_graphics, gc::PlayerColor::RED); TODO: Make AI

    int last_update_time_ms = SDL_GetTicks(); //[ms] time since SDL_Init was called
    //Start game loop
    while (true) {
        //Reset keys
        _input.beginNewFrame();

        //Check if events have occurred and store them
        if (SDL_PollEvent(&_event)) {
            if (_event.type == SDL_KEYDOWN) {
                //key was pressed
                if (_event.key.repeat == 0) {
                    //key is not being held down
                    _input.keyDownEvent(_event);
                }
            } else if (_event.type == SDL_KEYUP) {
                //key was released
                _input.keyUpEvent(_event);
            } else if (_event.type == SDL_QUIT) {
                //Quit game if its closed
                return;
            }
        }

        //Check if esc was pressed
        if (_input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) return;

        //Calculate elapsed time
        const int CURRENT_TIME_MS = SDL_GetTicks();
        int elapsed_frame_time_ms = CURRENT_TIME_MS - last_update_time_ms;
        _elapsedTime = std::min(elapsed_frame_time_ms, gc::MAX_FRAME_TIME_MS);
        last_update_time_ms = CURRENT_TIME_MS;

        //Update game
        update();        

        //Draw
        draw();
    }
}

void Zerkola::draw() {
    _graphics.clear();

    _tank_blue.draw(_graphics);

    _graphics.flip();
    return;    
}

void Zerkola::update() {    
    _tank_blue.update(_elapsedTime);
    return;
}

} //zerkola