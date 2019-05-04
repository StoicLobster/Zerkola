/* AnimatedSprite class
 * Animates sprites...
 */

#include <animated_sprite.h>
#include <game_control.h>
#include <geometry.h>

namespace animated_sprite {

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(graphics::Graphics& graphics, const std::string& filePath, int sourceX, int sourceY,
    int width, int height, double posX, double posY, double timeToUpdate):
    Sprite(graphics, filePath, sourceX, sourceY, width, height),
    _frameIdx(0),
    _timeElapsed(0.0),
    _visible(true),
    _timeToUpdate(timeToUpdate),
    _currentAnimationOnce(false),
    _currentAnimation("")       
    {}

void AnimatedSprite::_addAnimation(int frames, int sprite_x0, int sprite_y0, std::string name, int width, int height, bool reverse) {
    std::vector<SDL_Rect> rectangles;
    int start, end;
    if (reverse) {
        start = (frames - 1);
        end = 0;
    } else {
        start = 0;
        end = (frames - 1);
    }
    for (int i = start; i <= end; ++i) {
        SDL_Rect newRect = { sprite_x0 + i * width, sprite_y0, width, height };
        rectangles.push_back(newRect);
    }
    _animations.insert(std::pair< std::string, std::vector<SDL_Rect> >(name, rectangles));
    //_offsets.insert(std::pair<std::string, Eigen::Vector2d>(name,offset));
    return;
}

void AnimatedSprite::_resetAnimations() {
    _animations.clear();
    //_offsets.clear();
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

void AnimatedSprite::_setVisible(bool visible) {
    _visible = visible;
    return;
}

void AnimatedSprite::_stopAnimation() {
    _frameIdx = 0;
    _animationDone(_currentAnimation);
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

void AnimatedSprite::draw(graphics::Graphics& graphics) {
    if (_visible) {
        //Set current _sourceRect
        _sourceRect = _animations[_currentAnimation][_frameIdx];
        //Draw
        sprite::Sprite::draw(graphics);
    }
    return;
}

}