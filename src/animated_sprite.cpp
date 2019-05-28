#include <animated_sprite.h>
#include <game_control.h>
#include <geometry.h>
#include <iostream>

namespace animated_sprite {

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(
    graphics::Graphics* graphics_ptr, 
    const std::string& filePath, 
    int sourceX,
    int sourceY,
    int width, 
    int height, 
    double timeToUpdate):

sprite::Sprite(
    graphics_ptr, 
    filePath, 
    sourceX, 
    sourceY, 
    width, 
    height),

_frameIdx(0),
_timeToUpdate(timeToUpdate),
_timeElapsed(0.0),
_visible(true),
_currentAnimationOnce(false),
_currentAnimation("Idle")       
{
    #ifdef DEBUG_ANIMATED_SPRITE 
        std::cout << "===== AnimatedSprite Constructor Called =====" << std::endl;
    #endif
}

AnimatedSprite::~AnimatedSprite() {}

void AnimatedSprite::_addAnimation(
    int frames, 
    int sprite_x0, 
    int sprite_y0, 
    std::string name, 
    int width, 
    int height, 
    bool reverse) {
    #ifdef DEBUG_ANIMATED_SPRITE 
        std::cout << "AnimatedSprite::_addAnimation()" << std::endl;
    #endif
    std::vector<SDL_Rect> rectangles;
    if (reverse) {
        for (int i = (frames - 1); i >= 0; --i) {
            SDL_Rect newRect = { sprite_x0 + i * width, sprite_y0, width, height };
            rectangles.push_back(newRect);
        }
    } else {
        for (int i = 0; i <= (frames - 1); ++i) {
            SDL_Rect newRect = { sprite_x0 + i * width, sprite_y0, width, height };
            rectangles.push_back(newRect);
        }
    }

    _animations.insert(std::pair< std::string, std::vector<SDL_Rect> >(name, rectangles));
    return;
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
    _currentAnimationOnce = once;
    if (_currentAnimation != animation) {
        _currentAnimation = animation;
        _frameIdx = 0;
    }
    return;
}

void AnimatedSprite::update(int elapsedTime) {
    _timeElapsed += elapsedTime;
    if (_timeElapsed >= _timeToUpdate) {
        _timeElapsed -= _timeToUpdate;
        if (_frameIdx < _animations[_currentAnimation].size() - 1) {
            _frameIdx++;
        } else {
            if (_currentAnimationOnce) {
                _setVisible(false);
            }
            _frameIdx = 0;
            _animationDone(_currentAnimation);
        }

    }
    return;
}

void AnimatedSprite::draw(bool verbose) {
    if (verbose) std::cout << "AnimatedSprite::draw()" << std::endl;
    if (_visible) {
        //Set current _sourceRect
        _sourceRect = _animations[_currentAnimation][_frameIdx];
        if (verbose) std::cout << "Current Animation: " << _currentAnimation << ", Current Frame: " << _frameIdx << std::endl;
        //Draw
        sprite::Sprite::draw(verbose);
    }
    return;
}

} //namespace animated_sprite