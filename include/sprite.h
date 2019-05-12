#ifndef ZERKOLA_INCLUDE_SPRITE_H_
#define ZERKOLA_INCLUDE_SPRITE_H_

#include <SDL2/SDL.h>
#include <string>
#include <graphics.h>
#include <Eigen/Dense>

//#define DEBUG_SPRITE

namespace sprite {

class Sprite {
private:
Eigen::Vector2i _upper_left_corner; //Upper left corner of destination rectangle. BEFORE ROTATION
Eigen::Vector2d _dir; //Desired direction of destination rectangle
Eigen::Vector2i _center_of_rotation; //Center of rotation of rectangle RELATIVE TO UPPER LEFT

protected:
SDL_Rect _sourceRect; //location on sprite sheet to get sprite from
SDL_Texture* _spriteSheet; //Loaded sprite sheet

public:
Sprite();
Sprite(graphics::Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height);
virtual ~Sprite();
void draw(graphics::Graphics& graphics) const; //Draws sprite to graphics
inline void setUL(const Eigen::Vector2i& UL) { _upper_left_corner = UL; };
inline void setCOR(const Eigen::Vector2i& center) { _center_of_rotation = center; };
inline void setDirection(const Eigen::Vector2d& dir) { _dir = dir; };
};

}

#endif //ZERKOLA_INCLUDE_SPRITE_H_