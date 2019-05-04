/* Input class
 * Keeps track of keyboard and mouse state
 */

#include <input.h>

namespace input {

void Input::beginNewFrame() {
    _pressedKeys.clear();
    _releasedKeys.clear();
    return;
}

void Input::keyUpEvent(const SDL_Event& event) {
    _releasedKeys[event.key.keysym.scancode] = true;
    _heldKeys[event.key.keysym.scancode] = false;
    return;
}

void Input::keyDownEvent(const SDL_Event& event) {
    _pressedKeys[event.key.keysym.scancode] = true;
    _heldKeys[event.key.keysym.scancode] = true;
    return;
}

bool Input::wasKeyPressed(const SDL_Scancode& key) {
    return(_pressedKeys[key]); //Important note, when map is queried with [] for an element it does not contain, it adds a default element with that key (in this case false)
}

bool Input::wasKeyReleased(const SDL_Scancode& key) {
    return(_releasedKeys[key]);
}

bool Input::isKeyHeld(const SDL_Scancode& key) {
    return(_heldKeys[key]);
}

} //input