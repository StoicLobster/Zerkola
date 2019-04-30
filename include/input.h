#ifndef ZERKOLA_INCLUDE_INPUT_H
#define ZERKOLA_INCLUDE_INPUT_H

#include <map>
#include <SDL2/SDL.h>

namespace input {
class Input {
public:
    void beginNewFrame(); //gets called at the beginning of every new frame to reset irrelevant keys
    void keyUpEvent(const SDL_Event& event); //what happens when key is released
    void keyDownEvent(const SDL_Event& event); //what happens when key is pressed

    bool wasKeyPressed(const SDL_Scancode& key); //checks if certain key was pressed during frame
    bool wasKeyReleased(const SDL_Scancode& key); //checks if certain key was released during frame
    bool isKeyHeld(const SDL_Scancode& key); //checks if certain key is currently being held
private:
    std::map<SDL_Scancode, bool> _heldKeys; //keys that have been held
    std::map<SDL_Scancode, bool> _pressedKeys; //keys that have been pressed
    std::map<SDL_Scancode, bool> _releasedKeys; //keys that have been released
    //TODO: Add mouse input?
};
}

#endif //ZERKOLA_INCLUDE_INPUT_H