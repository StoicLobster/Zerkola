#ifndef ZERKOLA_INCLUDE_ANIMATED_SPRITE_H_
#define ZERKOLA_INCLUDE_ANIMATED_SPRITE_H_
//Includes
#include <string>
#include <map>
#include <vector>
#include <sprite.h>
#include <graphics.h>
#include <Eigen/Dense>

//#define DEBUG_ANIMATED_SPRITE

namespace animated_sprite {

/* Base class (ADT) for animated sprites */
class AnimatedSprite : public sprite::Sprite {

/*=== START PRIVATE ===*/
private:
    /* map of all animations for this sprite */
    std::map< std::string, std::vector<SDL_Rect> > _animations; //

    /* Current animation frame of sprite */
    unsigned int _frameIdx;

    /* [ms] Minimum time between animation updates */
    double _timeToUpdate;

    /* [ms] Time elapsed since last animation frame. Bounded by [0, _timeToUpdate] */
    double _timeElapsed;

    /* Controls if sprite is visible */
    bool _visible;

    /* True if animation should only play once, otherwise loops indefinitely */
    bool _currentAnimationOnce; //

    /* Name of current active animation */
    std::string _currentAnimation;
/*=== END PRIVATE ===*/

/*=== START PROTECTED ===*/
protected:
    /* Adds animation to map of animations */
    void _addAnimation(
        int frames, 
        int sprite_x0, 
        int sprite_y0, 
        std::string name, 
        int width, 
        int height, 
        bool reverse);
    
    /* Clears out map of animations */
    inline void _resetAnimations() { _animations.clear(); };
    
    /* Stops current animations */
    void _stopAnimation() { _frameIdx = 0; _animationDone(_currentAnimation); };
    
    /* Changes visibility of animated sprite */
    inline void _setVisible(bool visible) { _visible = visible; };
    
    /* Logic that happens when animation ends. Pure virtual. */
    virtual void _animationDone(std::string currentAnimation) = 0;
    
    /* Sets up all animations for a sprite. Pure virtual. */
    virtual void _setupAnimations() = 0;
/*=== END PROTECTED ===*/

/*=== START PUBLIC ===*/
public:
    AnimatedSprite();
    virtual ~AnimatedSprite();
    AnimatedSprite(
        graphics::Graphics* graphics_ptr, 
        const std::string& filePath, 
        int sourceX, 
        int sourceY, 
        int width, 
        int height, 
        double timeToUpdate);

    /* Plays the animation if its not already playing */
    void playAnimation(std::string animation, bool once = false);

    /* Updates the animated sprite timer and adjusts the sprite frame */
    void update(int elapsedTime);

    /* Draw sprite on screen at current position and orientation (of Sprite) */
    void draw(bool verbose=false);
/*=== END PUBLIC ===*/

}; //class AnimatedSprite

} //namespace animated_sprite

#endif //ZERKOLA_INCLUDE_ANIMATED_SPRITE_H_