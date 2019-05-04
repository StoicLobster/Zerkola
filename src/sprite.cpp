#include <sprite.h>
#include <iostream>
#include <game_control.h>
#include <algorithm>
#include <geometry.h>

namespace sprite {

Sprite::Sprite() {}; //TODO

Sprite::~Sprite() {}

Sprite::Sprite(graphics::Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height):
    _center(0,0),
    _dir(0,0)
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

void Sprite::draw(graphics::Graphics& graphics) {
    //Create destination rectangle on screen
    //This is where double is converted to 
    int UL_x, UL_y, W, H;
    W = static_cast<int>(_sourceRect.w*gc::SPRITE_SCALE);
    H = static_cast<int>(_sourceRect.h*gc::SPRITE_SCALE);
    UL_x = std::max(0 , _center.x() - gc::TANK_BODY_CENTER_RELATIVE_TO_UL_X);
    UL_y = std::max(0 , _center.y() - gc::TANK_BODY_CENTER_RELATIVE_TO_UL_Y);
    SDL_Rect destinationRectangle = {UL_x, UL_y, W, H};
    //Determine angle from global Y_2D (pointing up) to _dir CW
    Eigen::Vector2d v1((gc::Y_2D.cast<double>() * -1));
    Eigen::Vector2d v2(_dir.cast<double>());
    double theta = geo::AngBetweenVecs(v1, v2, gc::AngularDirections::CW)*geo::RAD_TO_DEG; //Note angle must be in degrees
    SDL_Point center_point = {_center.x(), _center.y()};
    //Render
    graphics.renderCopy(_spriteSheet, &_sourceRect, &destinationRectangle, theta, &center_point);
    return;
}

}