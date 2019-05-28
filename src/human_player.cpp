#include <human_player.h>

namespace human_player {

HumanPlayer::HumanPlayer(graphics::Graphics* graphics_ptr, 
    std::list<missile::Missile*>* missiles_ptr, 
    input::Input* input_ptr):

tank::Tank(graphics_ptr, gc::PlayerColor::BLUE, missiles_ptr),

_input_ptr(input_ptr)
{}

HumanPlayer::~HumanPlayer() {};

void HumanPlayer::_turn(double dt_ms) {
    #ifdef DEBUG_TANK 
        std::cout << "Tank::_turn()" << std::endl;
    #endif
    gc::LinearDirections translate_body_cmnd = gc::LinearDirections::LINEAR_NONE;
    gc::AngularDirections rotate_body_cmnd = gc::AngularDirections::ANGULAR_NONE;
    gc::AngularDirections rotate_turret_cmnd = gc::AngularDirections::ANGULAR_NONE;
    //Parse input and execute move / fire
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_SPACE)) {
        //Fire a Missile
        #ifdef DEBUG_TANK_FIRE 
            std::cout << "Fire!" << std::endl;
        #endif
        _fire(_missiles_ptr);
    }
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_UP) || _input_ptr->isKeyHeld(SDL_SCANCODE_UP)) {
        translate_body_cmnd = gc::LinearDirections::FORWARD;        
    } else if (_input_ptr->wasKeyPressed(SDL_SCANCODE_DOWN) || _input_ptr->isKeyHeld(SDL_SCANCODE_DOWN)) {
        translate_body_cmnd = gc::LinearDirections::REVERSE;
    }
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_RIGHT) || _input_ptr->isKeyHeld(SDL_SCANCODE_RIGHT)) {
        rotate_body_cmnd = gc::AngularDirections::CW;
    } else if (_input_ptr->wasKeyPressed(SDL_SCANCODE_LEFT) || _input_ptr->isKeyHeld(SDL_SCANCODE_LEFT)) {
        rotate_body_cmnd = gc::AngularDirections::CCW;
    }
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_D) || _input_ptr->isKeyHeld(SDL_SCANCODE_D)) {
        rotate_turret_cmnd = gc::AngularDirections::CW;
    } else if (_input_ptr->wasKeyPressed(SDL_SCANCODE_A) || _input_ptr->isKeyHeld(SDL_SCANCODE_A)) {
        rotate_turret_cmnd = gc::AngularDirections::CCW;
    }
    _move(dt_ms, translate_body_cmnd, rotate_body_cmnd, rotate_turret_cmnd);

    
    return;
}

} //namespace human_player