/* Graphics class
 * Holds all information dealing with graphics for the game
 */
#ifndef ZERKOLA_INCLUDE_GRAPHICS_H_
#define ZERKOLA_INCLUDE_GRAPHICS_H_

#include <map>
#include <string>
#include <SDL2/SDL_image.h>

//#define DEBUG_GRAPHICS

namespace graphics {

/* Window, game board, and renderer  */
class Graphics {

/*=== START PRIVATE ===*/
private:
    /* Window where game is drawn */
    SDL_Window* _window;

    /* Renderer which performs drawing on window */
    SDL_Renderer* _renderer;

    /* Map of sprites used */
    std::map<std::string, SDL_Surface*> _spriteSheets;

    /* Board where game is played (within window) */
    SDL_Rect _board;
/*=== END PRIVATE ===*/

/*=== START PUBLIC ===*/
public:
    Graphics();
    ~Graphics();
    /* Load image into _spriteSheets map if it doesnt exist already */
    SDL_Surface* loadImage(const std::string& filePath);

    /* Uses SDL_RenderCopyEx to render and rotate the given texture */    
    void renderCopy(
        SDL_Texture* texture, 
        const SDL_Rect* sourceRectangle, 
        const SDL_Rect* destinationRectangle, 
        const double angle, 
        const SDL_Point* center);
    /* Renders everything to screen */
    inline void flip() { SDL_RenderPresent(_renderer); };

    /* Resets the screen to its init state */
    void clear();

    /* Return _renderer */
    SDL_Renderer* getRenderer() const;

    /* Hide window */
    inline void hideWindow() const { SDL_HideWindow(_window); };

    /* Show window */
    inline void showWindow() const { SDL_ShowWindow(_window); };

    /* Raise window above others and set input focus */
    inline void raiseWindow() const { SDL_RaiseWindow(_window); };
/*=== END PUBLIC ===*/

}; //class Graphics

} //namespace graphics

#endif //ZERKOLA_INCLUDE_GRAPHICS_H_
