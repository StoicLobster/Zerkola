/* AnimatedSprite class
 * Animates sprites...
 */

#include <animated_sprite.h>
#include <game_control.h>

namespace animated_sprite {

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(graphics::Graphics& graphics, const std::string& filePath, int sourceX, int sourceY,
    int width, int height, double posX, double posY, double timeToUpdate):
    Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
    _timeToUpdate(timeToUpdate),
    _currentAnimationOnce(false),
    _currentAnimation(""),
    _frameIdx(0),    
    _visible(true)   
    {}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Eigen::Vector2d offset, bool reverse) {
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
        SDL_Rect newRect = { x + i * width, y, width, height };
        rectangles.push_back(newRect);
    }
    _animations.insert(std::pair< std::string, std::vector<SDL_Rect> >(name, rectangles));
    _offsets.insert(std::pair<std::string, Eigen::Vector2d>(name,offset));
    return;
}

void AnimatedSprite::resetAnimations() {
    _animations.clear();
    _offsets.clear();
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

void AnimatedSprite::setVisible(bool visible) {
    _visible = visible;
    return;
}

void AnimatedSprite::stopAnimation() {
    _frameIdx = 0;
    animationDone(_currentAnimation);
    return;
}

void AnimatedSprite::update(int elapsedTime) {
    sprite::Sprite::update();
    _timeElapsed += elapsedTime;
    if (_timeElapsed >= _timeToUpdate) {
        _timeElapsed -= _timeToUpdate;
        if (_frameIdx < _animations[_currentAnimation].size() - 1) {
            _frameIdx++;
        } else {
            if (_currentAnimationOnce) {
                setVisible(false);
            }
            _frameIdx = 0;
            animationDone(_currentAnimation);
        }

    }
    return;
}

void AnimatedSprite::draw(graphics::Graphics& graphics, int x, int y) {
    if (_visible) {
        SDL_Rect destinationRectangle;
        destinationRectangle.x = x + _offsets[_currentAnimation].x();
        destinationRectangle.y = y + _offsets[_currentAnimation].y();
        destinationRectangle.w = static_cast<int>(_sourceRect.w * gc::SPRITE_SCALE);
        destinationRectangle.h = static_cast<int>(_sourceRect.h * gc::SPRITE_SCALE);

        SDL_Rect sourceRect = _animations[_currentAnimation][_frameIdx];
        graphics.blitSurface(_spriteSheet, &sourceRect, &destinationRectangle);
    }
    return;
}

//void AnimatedSprite::animationDone(std::string currentAnimation) {}

// void AnimatedSprite::setupAnimations() {    
//     
//     return;
// }

}