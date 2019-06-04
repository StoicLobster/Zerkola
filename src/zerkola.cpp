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
    _event(),
    _winningPlayer(gc::PlayerColor::PLAYER_COLOR_NONE)
{
    bool success = setup(); //setup game
    if (success) loop(); //begin game
}

Zerkola::~Zerkola() {
    //Delete tanks
    delete _tank_red;
    delete _tank_blue;
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
    bool valid_inpt = false;
    while (!valid_inpt) {
        std::cout << "Selection: ";
        std::cin >> inpt;
        if (std::cin.fail() || 
            (inpt <= gc::PlayerType::PLAYER_TYPE_NONE) || 
            (inpt >= gc::PlayerType::NUM_PLAYER_TYPE) || 
            gc::ComputerPlayerList.find(static_cast<gc::PlayerType>(inpt)) == gc::ComputerPlayerList.end()) {
            //Invalid Input
            std::cout << "Please enter an integer selection from the list." << std::endl;
            // clear error state
            std::cin.clear();
            // discard 'bad' character(s)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else valid_inpt = true;        
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
    _graphics.showWindow();
    _graphics.raiseWindow();
    return(true);
}

void Zerkola::loop() {         
    int last_update_time_ms = SDL_GetTicks(); //[ms] time since SDL_Init was called
    //Start game loop
    while (_winningPlayer == gc::PlayerColor::PLAYER_COLOR_NONE) {
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
    end();
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
    if (_tank_blue->update(_elapsedTime)) {
        _winningPlayer = gc::PlayerColor::RED;
        return;
    } 
    if (_tank_red->update(_elapsedTime)) {
        _winningPlayer = gc::PlayerColor::BLUE;
        return;
    }
    for (auto missile : _missiles) {
        missile->updateMissile(_elapsedTime);
    }
    return;
}

void Zerkola::end() {
    if (_winningPlayer == gc::PlayerColor::BLUE) std::cout << "You won! Yay." << std::endl;
    else std::cout << "You lost. Boo." << std::endl;
    return;
}

} //zerkola