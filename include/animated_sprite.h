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
public:
    AnimatedSprite();
    AnimatedSprite(graphics::Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, 
        double posX, double posY, double timeToUpdate);
    void playAnimation(std::string animation, bool once = false); //plays the animation if its not already playing

    void update(int elapsedTime); //updates the animated sprite timer

    void draw(graphics::Graphics& graphics, int x, int y); //draw sprite on screen

    virtual void setupAnimations(); //required function that sets up all animations for a sprite

protected:
    double _timeToUpdate; //amount of time between frames
    bool _currentAnimationOnce;
    std::string _currentAnimation; //name of current animation

    void addAnimation(int frames, int x, int y, std::string name, int width, int height, Eigen::Vector2d offset); //adds animation to map of animations

    void resetAnimations(); //clears out map of animations

    void stopAnimation(); //stops current animations

    void setVisible(bool visible); //changes visibility of animated sprite

    virtual void animationDone(std::string currentAnimation); //Logic that happens when animation ends

private:
    std::map< std::string, std::vector<SDL_Rect> > _animations;
    std::map< std::string, Eigen::Vector2d > _offsets;

    unsigned int _frameIdx;
    double _timeElapsed;
    bool _visible;
};

}

#endif //ZERKOLA_INCLUDE_ANIMATED_SPRITE_H_