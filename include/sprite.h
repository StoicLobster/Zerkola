#ifndef ZERKOLA_INCLUDE_SPRITE_H_
#define ZERKOLA_INCLUDE_SPRITE_H_

#include <SDL2/SDL.h>
#include <string>
#include <graphics.h>
#include <Eigen/Dense>

#define DEBUG_SPRITE

namespace sprite {

class Sprite {
private:
Eigen::Vector2i _center; //Current center location of sprite on screen
Eigen::Vector2i _center_to_UL; //Shift from _center to upper left of bounding box
Eigen::Vector2d _dir; //Current direction of sprite on screen

protected:
SDL_Rect _sourceRect; //location on sprite sheet to get sprite from
SDL_Texture* _spriteSheet; //Loaded sprite sheet

public:
Sprite();
Sprite(graphics::Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, int UL_x, int UL_y);
virtual ~Sprite();
void draw(graphics::Graphics& graphics) const; //Draws sprite to graphics with _center and _dir
inline void setCenter(const Eigen::Vector2i& center) { _center = center; };
inline void setDirection(const Eigen::Vector2d& dir) { _dir = dir; };
};

}

#endif //ZERKOLA_INCLUDE_SPRITE_H_