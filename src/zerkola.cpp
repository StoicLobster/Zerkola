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

    _player_A = animated_sprite::AnimatedSprite(graphics, "content/sprites/red_tank_2.png", 0, 0, 29, 31, 100, 100, 100);
    _player_A.setupAnimations();
    _player_A.playAnimation("RollForward");

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

    _player_A.draw(graphics, 100, 100);

    graphics.flip();
    return;    
}

void Zerkola::update(double elaspedTime) {
    _player_A.update(elaspedTime);
    return;
}

} //zerkola