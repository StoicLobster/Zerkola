/* Graphics class
 * Holds all information dealing with graphics for the game
 */
#include <graphics.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <game_control.h>
#include <iostream>

namespace graphics {

Graphics::Graphics():
    _window(nullptr),
    _renderer(nullptr)
{
    _window = SDL_CreateWindow(gc::GAME_TITLE, 100, 100, gc::WINDOW_WIDTH, gc::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if (_window == nullptr) {
        std::cout << "Error could not create window" << SDL_GetError() << std::endl;
    }
    #ifdef DEBUG_GRAPHICS
        std::cout << "Graphics::Graphics Constructor" << std::endl;
    #endif
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
    if (SDL_RenderCopyEx(_renderer, texture, sourceRectangle, destinationRectangle, angle, center, SDL_RendererFlip::SDL_FLIP_NONE)) {
        std::cout << SDL_GetError() << std::endl;
    }
    return;
}

void Graphics::flip() {
    #ifdef DEBUG_GRAPHICS
        //std::cout << "Graphics::flip()" << std::endl;
    #endif
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

