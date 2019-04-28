#include <sprite.h>
#include <iostream>
#include <game_control.h>

namespace sprite {

Sprite::Sprite() {}; //TODO

Sprite::Sprite(graphics::Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, double posX, double posY):
    _x(posX),
    _y(posY) 
{
    _sourceRect.x = sourceX;
    _sourceRect.y = sourceY;
    _sourceRect.w = width;
    _sourceRect.h = height;

    _spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
    if (_spriteSheet == NULL) {
        std::cout << std::endl << "Error: Unable to load image" << std::endl;
    }
}

Sprite::~Sprite() {}

void Sprite::draw(graphics::Graphics& graphics, int x, int y) {
    SDL_Rect destinationRectangle = {x, y, static_cast<int>(_sourceRect.w*gc::SPRITE_SCALE), static_cast<int>(_sourceRect.h*gc::SPRITE_SCALE)};
    graphics.blitSurface(_spriteSheet, &_sourceRect, &destinationRectangle);
    return;
}

void Sprite::update() {}

}