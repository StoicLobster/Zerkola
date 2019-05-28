#ifndef ZERKOLA_INCLUDE_SPRITE_H_
#define ZERKOLA_INCLUDE_SPRITE_H_
//Includes
#include <SDL2/SDL.h>
#include <string>
#include <graphics.h>
#include <Eigen/Dense>

//#define DEBUG_SPRITE

namespace sprite {

/* Base sprite class */
class Sprite {

/*=== START PRIVATE ===*/
private:
    /* Graphics used for drawing sprite */
    graphics::Graphics* _graphics_ptr;

    /* Upper left corner of destination rectangle (NOTE: before rotation) */
    Eigen::Vector2i _upper_left_corner;

    /* Desired direction of destination rectangle */
    Eigen::Vector2d _dir;

    /* Center of rotation of rectangle (NOTE: relative to upper left corner) */
    Eigen::Vector2i _center_of_rotation;
/*=== END PRIVATE ===*/

/*=== START PROTECTED ===*/
protected:
    /* Location on sprite sheet to get sprite from */
    SDL_Rect _sourceRect;

    /* Loaded sprite sheet */
    SDL_Texture* _spriteSheet;
/*=== END PROTECTED ===*/

/*=== START PUBLIC ===*/
public:
    Sprite();
    ~Sprite();
    Sprite(graphics::Graphics* graphics_ptr, const std::string &filePath, int sourceX, int sourceY, int width, int height);
    
    /* Draws sprite to graphics */
    void draw(bool verbose=false) const;

    /* Set upper left corner */
    inline void setUL(const Eigen::Vector2i& UL) { _upper_left_corner = UL; };

    /* Set center of rotation */
    inline void setCOR(const Eigen::Vector2i& center) { _center_of_rotation = center; };

    /* Set direction */
    inline void setDirection(const Eigen::Vector2d& dir) { _dir = dir; };
/*=== END PUBLIC ===*/

}; //class Sprite

} //namespace sprite

#endif //ZERKOLA_INCLUDE_SPRITE_H_