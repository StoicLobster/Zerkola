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

Sprite::Sprite(graphics::Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height):
    _upper_left_corner(0,0),
    _dir(0,0),
    _center_of_rotation(0,0)    
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
        std::cout << "_upper_left_corner.x(): " << _upper_left_corner.x() << std::endl;
        std::cout << "_upper_left_corner.y(): " << _upper_left_corner.y() << std::endl;
        std::cout << "_dir.x(): " << _dir.x() << std::endl;
        std::cout << "_dir.y(): " << _dir.y() << std::endl;
        std::cout << "_center_of_rotation.x(): " << _center_of_rotation.x() << std::endl;
        std::cout << "_center_of_rotation.y(): " << _center_of_rotation.y() << std::endl;
    #endif
    //Create destination rectangle on screen
    //This is where double is converted to 
    int UL_x, UL_y, width, height;
    width = static_cast<int>(_sourceRect.w*gc::SPRITE_SCALE);
    height = static_cast<int>(_sourceRect.h*gc::SPRITE_SCALE);
    UL_x = _upper_left_corner.x();
    UL_y = _upper_left_corner.y();
    #ifdef DEBUG_SPRITE 
        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;
        std::cout << "UL_x: " << UL_x << std::endl;
        std::cout << "UL_y: " << UL_y << std::endl;
    #endif
    //Assert on window limits
    assert((UL_x + width) <= gc::WINDOW_WIDTH);
    assert(UL_x >= 0);
    assert(UL_y >= 0);
    assert((UL_y + height) <= gc::WINDOW_HEIGHT);
    SDL_Rect destinationRectangle = {UL_x, UL_y, width, height};
    //Determine angle from global Y_2D (pointing up) to _dir CW
    Eigen::Vector2d v1(gc::Y_2D.cast<double>());
    Eigen::Vector2d v2(_dir.cast<double>());
    double theta = geo::AngBetweenVecs(v1, v2, gc::AngularDirections::CW)*geo::RAD_TO_DEG; //Note angle must be in degrees
    #ifdef DEBUG_SPRITE 
        std::cout << "theta: " << theta << std::endl;
    #endif
    SDL_Point center_point = {_center_of_rotation.x(), _center_of_rotation.y()};
    //Render
    graphics.renderCopy(_spriteSheet, &_sourceRect, &destinationRectangle, theta, &center_point);
    return;
}

}