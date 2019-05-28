#ifndef ZERKOLA_INCLUDE_INPUT_H
#define ZERKOLA_INCLUDE_INPUT_H
//Includes
#include <map>
#include <SDL2/SDL.h>

namespace input {

/* Keys pressed this frame */
class Input {

/*=== START PRIVATE ===*/
private:
    /* Keys that have been held this frame */
    std::map<SDL_Scancode, bool> _heldKeys;

    /* Keys that have been pressed this frame */
    std::map<SDL_Scancode, bool> _pressedKeys;

    /* Keys that have been released this frame */
    std::map<SDL_Scancode, bool> _releasedKeys;
/*=== END PRIVATE ===*/

/*=== START PUBLIC ===*/
public:
    /* Called at the beginning of every new frame to reset keys */
    void beginNewFrame();

    /* Event when key is released */
    void keyUpEvent(const SDL_Event& event);

    /* Event when key is pressed */
    void keyDownEvent(const SDL_Event& event);

    /* Checks if certain key was pressed during this frame */
    inline bool wasKeyPressed(const SDL_Scancode& key) { return(_pressedKeys[key]); };

    /* Checks if certain key was released during this frame */
    inline bool wasKeyReleased(const SDL_Scancode& key) { return(_releasedKeys[key]); };

    /* Checks if certain key is currently being held this frame */
    inline bool isKeyHeld(const SDL_Scancode& key) { return(_heldKeys[key]); };
/*=== END PUBLIC ===*/

}; //class Input

} //namespace input

#endif //ZERKOLA_INCLUDE_INPUT_H