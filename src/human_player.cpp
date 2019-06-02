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
    //Parse input and execute move / fire
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_SPACE)) {
        //Fire a Missile
        _fire(_missiles_ptr);
    }
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_UP) || _input_ptr->isKeyHeld(SDL_SCANCODE_UP)) {
        _translate_body_cmnd = gc::LinearDirections::FORWARD;        
    } else if (_input_ptr->wasKeyPressed(SDL_SCANCODE_DOWN) || _input_ptr->isKeyHeld(SDL_SCANCODE_DOWN)) {
        _translate_body_cmnd = gc::LinearDirections::REVERSE;
    }
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_RIGHT) || _input_ptr->isKeyHeld(SDL_SCANCODE_RIGHT)) {
        _rotate_body_cmnd = gc::AngularDirections::CW;
    } else if (_input_ptr->wasKeyPressed(SDL_SCANCODE_LEFT) || _input_ptr->isKeyHeld(SDL_SCANCODE_LEFT)) {
        _rotate_body_cmnd = gc::AngularDirections::CCW;
    }
    if (_input_ptr->wasKeyPressed(SDL_SCANCODE_D) || _input_ptr->isKeyHeld(SDL_SCANCODE_D)) {
        _rotate_turret_cmnd = gc::AngularDirections::CW;
    } else if (_input_ptr->wasKeyPressed(SDL_SCANCODE_A) || _input_ptr->isKeyHeld(SDL_SCANCODE_A)) {
        _rotate_turret_cmnd = gc::AngularDirections::CCW;
    }
    
    return;
}

} //namespace human_player