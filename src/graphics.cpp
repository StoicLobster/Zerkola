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
    _window = SDL_CreateWindow(gc::GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gc::WINDOW_WIDTH, gc::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if (_window == nullptr) {
        std::cout << "Error could not create window" << SDL_GetError() << std::endl;
    }
    //Define Board
    _board.x = gc::BOARD_WDW_LEFT;
    _board.y = gc::BOARD_WDW_TOP;
    _board.w = gc::BOARD_WDW_RIGHT - gc::BOARD_WDW_LEFT;
    _board.h = gc::BOARD_WDW_BOTTOM - gc::BOARD_WDW_TOP;
    clear();
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
    //Make margin black
    SDL_SetRenderDrawColor(_renderer, gc::COLOR_MRGN_R, gc::COLOR_MRGN_G, gc::COLOR_MRGN_B, gc::COLOR_MRGN_A);
    SDL_RenderClear(_renderer); //Fills screen with current RenderDrawColor
    //Set Board Color
    SDL_SetRenderDrawColor(_renderer, gc::COLOR_BOARD_R, gc::COLOR_BOARD_G, gc::COLOR_BOARD_B, gc::COLOR_BOARD_A);
    SDL_RenderFillRect(_renderer, &_board);
    //Render
    //SDL_RenderPresent(_renderer);
    return;
}

SDL_Renderer* Graphics::getRenderer() const {
    return(_renderer);
}

} //graphics

