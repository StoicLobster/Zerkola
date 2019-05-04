/* Graphics class
 * Holds all information dealing with graphics for the game
 */
#include <graphics.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <game_control.h>

namespace graphics {

Graphics::Graphics() {
    SDL_CreateWindowAndRenderer(gc::WINDOW_WIDTH, gc::WINDOW_HEIGHT, 0, &_window, &_renderer);
    SDL_SetWindowTitle(_window, gc::GAME_TITLE);
}

Graphics::~Graphics() {
    SDL_DestroyWindow(_window);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
    if (_spriteSheets.count(filePath) == 0) {
        _spriteSheets[filePath] = IMG_Load(filePath.c_str());
    }
    return (_spriteSheets[filePath]);
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
    SDL_RenderCopy(_renderer, texture, sourceRectangle, destinationRectangle);
    return;
}

void Graphics::renderCopy(SDL_Texture* texture, const SDL_Rect* sourceRectangle, const SDL_Rect* destinationRectangle, const double angle, const SDL_Point* center) {
    SDL_RenderCopyEx(_renderer, texture, sourceRectangle, destinationRectangle, angle, center, SDL_RendererFlip::SDL_FLIP_NONE);
    return;
}

void Graphics::flip() {
    SDL_RenderPresent(_renderer);
    return;
}

void Graphics::clear() {
    SDL_RenderClear(_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
    return(_renderer);
}

} //graphics

