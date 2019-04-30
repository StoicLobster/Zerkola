#include <player.h>
#include <game_control.h>

namespace player {

Player::Player() {}

Player::Player(graphics::Graphics& graphics, double x, double y):
    animated_sprite::AnimatedSprite(
        graphics, 
        gc::TANK_SPRITE_ANIMATION_LOCATION, 
        gc::RED_TANK_SPRITE_START_X, 
        gc::RED_TANK_SPRITE_START_Y, 
        gc::TANK_SPRITE_WIDTH, 
        gc::TANK_SPRITE_HEIGHT,
        x, 
        y, 
        gc::TANK_SPRITE_UPDATE_RATE_MS)
{
    graphics.loadImage(gc::TANK_SPRITE_ANIMATION_LOCATION);

    setupAnimations();
    playAnimation("RollForward");
}

void Player::setupAnimations() {
    addAnimation(gc::NUMBER_TANK_SPRITE_ROLL_ANIMATIONS, gc::RED_TANK_SPRITE_START_X, gc::RED_TANK_SPRITE_START_Y, "RollForward", gc::TANK_SPRITE_WIDTH, gc::TANK_SPRITE_HEIGHT, Eigen::Vector2d(0,0), false);
    addAnimation(gc::NUMBER_TANK_SPRITE_ROLL_ANIMATIONS, gc::RED_TANK_SPRITE_START_X, gc::RED_TANK_SPRITE_START_Y, "RollBackward", gc::TANK_SPRITE_WIDTH, gc::TANK_SPRITE_HEIGHT, Eigen::Vector2d(0,0), true);
    return;
}

void Player::animationDone(std::string currentAnimation) {};

void Player::update(double elapsedTime) {
    animated_sprite::AnimatedSprite::update(elapsedTime);
    return;
}

void Player::draw(graphics::Graphics &graphics) {
    animated_sprite::AnimatedSprite::draw(graphics, _x, _y);
    return;
}

}