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
    graphics::Graphics graphics;
    input::Input input;
    SDL_Event event; //Events that occur will be stored here by SDL

    _player_red = player::Player(graphics, gc::PLAYER_START_POS_X, gc::PLAYER_START_POS_Y);

    int last_update_time_ms = SDL_GetTicks(); //[ms] time since SDL_Init was called
    //Start game loop
    while (true) {
        //Reset keys
        input.beginNewFrame();

        //Check if events have occurred
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                //key was pressed
                if (event.key.repeat == 0) {
                    //key is not being held down
                    input.keyDownEvent(event);
                }
            } else if (event.type == SDL_KEYUP) {
                //key was released
                input.keyUpEvent(event);
            } else if (event.type == SDL_QUIT) {
                //Quit game if its closed
                return;
            }
        }

        //Check if esc was pressed
        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) return;

        const int CURRENT_TIME_MS = SDL_GetTicks();
        int elapsed_frame_time_ms = CURRENT_TIME_MS - last_update_time_ms;
        update(std::min(elapsed_frame_time_ms, gc::MAX_FRAME_TIME_MS));
        last_update_time_ms = CURRENT_TIME_MS;

        //Draw
        draw(graphics);
    }
}

void Zerkola::draw(graphics::Graphics &graphics) {
    graphics.clear();

    _player_red.draw(graphics);

    graphics.flip();
    return;    
}

void Zerkola::update(double elaspedTime) {
    _player_red.update(elaspedTime);
    return;
}

} //zerkola