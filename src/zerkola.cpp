/* Zerkola class
 * Home for Zerkola game itself
 */

#include <zerkola.h>
#include <game_control.h>
#include <SDL2/SDL.h>
#include <input.h>
#include <algorithm>
#include <iostream>
#include <human_player.h>

namespace zerkola {

Zerkola::Zerkola():
    _graphics(),
    _input(),
    _event()
{
    bool success = setup(); //setup game
    if (success) loop(); //begin game
}

Zerkola::~Zerkola() {
    //Technically Tank class allocates missile memory, but cleanup makes more sense here as Zerkola is the owner of missile list
    for (auto missile : _missiles) {
        delete missile;
    }
}

bool Zerkola::setup() {
    std::cout << "Welcome to Zerkola!" << std::endl;
    std::cout << "What computer would you like to play against?" << std::endl;
    std::cout << std::endl;
    for (auto& ele : gc::ComputerPlayerList) {
        std::cout << ele.second << std::endl;
    }
    std::cout << std::endl;
    int inpt = 0; //Human
    while (gc::ComputerPlayerList.find(static_cast<gc::PlayerType>(inpt)) == gc::ComputerPlayerList.end()) {
        std::cout << "Please select a computer player number from the list." << std::endl;
        std::cin >> inpt;
    }
    gc::PlayerType comp_player = static_cast<gc::PlayerType>(inpt);
    //Instantiate players
    _tank_blue = new human_player::HumanPlayer(&_graphics, &_missiles, &_input);
    switch (comp_player) {
        case gc::PlayerType::COMPUTER_R2D2:
            _tank_red = new r2d2::R2D2(&_graphics, gc::PlayerColor::RED, &_missiles);
            break;
        case gc::PlayerType::COMPUTER_SKYNET:
            _tank_red = new r2d2::R2D2(&_graphics, gc::PlayerColor::RED, &_missiles); //FIXME
            break;
        default:
            return(false);
            break;
    }
    std::cout << "GLHF!" << std::endl;
    return(true);
}

void Zerkola::loop() {         
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

        //Delay
        double delay_duration = std::max(0.0, static_cast<double>(gc::MAX_FRAME_TIME_MS) - _elapsedTime);
        SDL_Delay(delay_duration);
    }
}

void Zerkola::draw() {
    #ifdef DEBUG_ZERKOLA 
        std::cout << "Zerkola::draw()" << std::endl;
    #endif
    _graphics.clear();

    //Draw Missiles
    for (auto missile : _missiles) {
        missile->draw();
    }

    //Draw Tanks
    _tank_blue->drawTank();
    _tank_red->drawTank();

    _graphics.flip();
    return;    
}

void Zerkola::update() {    
    _tank_blue->updateTank(_elapsedTime);
    _tank_red->updateTank(_elapsedTime);
    for (auto missile : _missiles) {
        missile->updateMissile(_elapsedTime);
    }
    return;
}

} //zerkola