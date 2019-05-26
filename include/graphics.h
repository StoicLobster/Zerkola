/* Graphics class
 * Holds all information dealing with graphics for the game
 */
#ifndef ZERKOLA_INCLUDE_GRAPHICS_H_
#define ZERKOLA_INCLUDE_GRAPHICS_H_

#include <map>
#include <string>
#include <SDL2/SDL_image.h>

//#define DEBUG_GRAPHICS

struct SDL_Window;
struct SDL_Renderer; //Dont need to include SDL since pointers are used below.

namespace graphics {

class Graphics {
private:
    SDL_Window* _window; //Where game is drawn to
    SDL_Renderer* _renderer; //What does the drawing to the window
    std::map<std::string, SDL_Surface*> _spriteSheets;
    SDL_Rect _board; //Board is window less the margins. This is where game is played
    
public:
    Graphics();
    ~Graphics();

    SDL_Surface* loadImage(const std::string& filePath); //load image into _spriteSheets map if it doesnt already exist

    void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle); //draws SDL_Texture to screen. Not used
    /* Render Copy
     * Uses SDL_RenderCopyEx to render and rotate the given texture
     */    
    void renderCopy(
        SDL_Texture* texture, 
        const SDL_Rect* sourceRectangle, 
        const SDL_Rect* destinationRectangle, 
        const double angle, 
        const SDL_Point* center);
    void flip(); //renders everything to screen
    void clear(); //clears the screen

    SDL_Renderer* getRenderer() const; //Return _renderer
};

} //graphics

#endif //ZERKOLA_INCLUDE_GRAPHICS_H_
