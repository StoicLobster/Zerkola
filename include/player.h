#ifndef ZERKOLA_INCLUDE_PLAYER_H_
#define ZERKOLA_INCLUDE_PLAYER_H_

#include <graphics.h>
#include <animated_sprite.h>
#include <string.h>

namespace player {

class Player : public animated_sprite::AnimatedSprite {

public:
    Player();
    Player(graphics::Graphics& graphics, double x, double y);
    void draw(graphics::Graphics& graphics);
    void update(double elapsedTime);
    
    virtual void animationDone(std::string currentAnimation);
    virtual void setupAnimations();

private:
    double _dx, _dy; //Change in position during this frame

};

}

#endif