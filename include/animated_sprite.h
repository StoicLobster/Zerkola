#ifndef ZERKOLA_INCLUDE_ANIMATED_SPRITE_H_
#define ZERKOLA_INCLUDE_ANIMATED_SPRITE_H_

#include <string>
#include <map>
#include <vector>
#include <sprite.h>
#include <graphics.h>
#include <Eigen/Dense>

namespace animated_sprite {

class AnimatedSprite : public sprite::Sprite {
private:
std::map< std::string, std::vector<SDL_Rect> > _animations; //map of all animations for this sprite
//std::map< std::string, Eigen::Vector2d > _offsets;
unsigned int _frameIdx;
double _timeElapsed;
bool _visible;

protected:
double _timeToUpdate; //amount of time between frames
bool _currentAnimationOnce; //True if want to only show animation once, otherwise loops
std::string _currentAnimation; //name of current active animation

void _addAnimation(int frames, int sprite_x0, int sprite_y0, std::string name, int width, int height, bool reverse); //adds animation to map of animations
void _resetAnimations(); //clears out map of animations
void _stopAnimation(); //stops current animations
void _setVisible(bool visible); //changes visibility of animated sprite
virtual void _animationDone(std::string currentAnimation) = 0; //Logic that happens when animation ends. Pure virtual.
virtual void _setupAnimations() = 0; //Required function that sets up all animations for a sprite. Pure virtual.

public:
AnimatedSprite();
AnimatedSprite(graphics::Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, 
    double posX, double posY, double timeToUpdate);

void playAnimation(std::string animation, bool once = false); //plays the animation if its not already playing
void update(int elapsedTime); //updates the animated sprite timer and adjusts the sprite frame
void draw(graphics::Graphics& graphics); //draw sprite on screen at current position and orientation (of Sprite)
};

}

#endif //ZERKOLA_INCLUDE_ANIMATED_SPRITE_H_