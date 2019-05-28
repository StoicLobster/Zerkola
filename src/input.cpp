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

} //namespace input