#include <sprite.h>
#include <iostream>
#include <game_control.h>
#include <algorithm>
#include <geometry.h>
#include <iostream>
#include <assert.h>

namespace sprite {

Sprite::Sprite() {}; //TODO

Sprite::~Sprite() {}

Sprite::Sprite(graphics::Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, int UL_x, int UL_y):
    _center(0,0),
    _center_to_UL(UL_x, UL_y),
    _dir(0,0)
{
    #ifdef DEBUG_SPRITE 
        std::cout << "===== Sprite Constructor Called =====" << std::endl;
    #endif
    _sourceRect.x = sourceX;
    _sourceRect.y = sourceY;
    _sourceRect.w = width;
    _sourceRect.h = height;
    #ifdef DEBUG_SPRITE 
        std::cout << "_sourceRect.x: " << _sourceRect.x << std::endl;
        std::cout << "_sourceRect.y: " << _sourceRect.y << std::endl;
        std::cout << "_sourceRect.w: " << _sourceRect.w << std::endl;
        std::cout << "_sourceRect.h: " << _sourceRect.h << std::endl;
    #endif

    _spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
    if (_spriteSheet == NULL) {
        std::cout << std::endl << "Error: Unable to load image" << std::endl;
    }
}

void Sprite::draw(graphics::Graphics& graphics) const {
    #ifdef DEBUG_SPRITE 
        std::cout << "Sprite::draw()" << std::endl;
        std::cout << "_center.x(): " << _center.x() << std::endl;
        std::cout << "_center.y(): " << _center.y() << std::endl;
        std::cout << "_dir.x(): " << _dir.x() << std::endl;
        std::cout << "_dir.y(): " << _dir.y() << std::endl;
    #endif
    //Create destination rectangle on screen
    //This is where double is converted to 
    int UL_x, UL_y, width, height;
    width = static_cast<int>(_sourceRect.w*gc::SPRITE_SCALE);
    height = static_cast<int>(_sourceRect.h*gc::SPRITE_SCALE);
    UL_x = _center.x() + _center_to_UL.x();
    UL_y = _center.y() + _center_to_UL.y();
    #ifdef DEBUG_SPRITE 
        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;
        std::cout << "UL_x: " << UL_x << std::endl;
        std::cout << "UL_y: " << UL_y << std::endl;
    #endif
    SDL_Rect destinationRectangle = {UL_x, UL_y, width, height};
    //Determine angle from global Y_2D (pointing up) to _dir CW
    Eigen::Vector2d v1(gc::Y_2D.cast<double>());
    Eigen::Vector2d v2(_dir.cast<double>());
    double theta = geo::AngBetweenVecs(v1, v2, gc::AngularDirections::CW)*geo::RAD_TO_DEG; //Note angle must be in degrees
    #ifdef DEBUG_SPRITE 
        std::cout << "theta: " << theta << std::endl;
    #endif
    SDL_Point center_point = {-1*_center_to_UL.x(), -1*_center_to_UL.y()};
    //Assert on window limits
    assert(_center.x() <= gc::WINDOW_WIDTH);
    assert(_center.x() >= 0);
    assert(_center.y() >= 0);
    assert(_center.y() <= gc::WINDOW_HEIGHT);
    //Render
    graphics.renderCopy(_spriteSheet, &_sourceRect, &destinationRectangle, theta, &center_point);
    return;
}

}