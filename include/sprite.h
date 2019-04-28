#ifndef ZERKOLA_INCLUDE_SPRITE_H_
#define ZERKOLA_INCLUDE_SPRITE_H_

#include <SDL2/SDL.h>
#include <string>
#include <graphics.h>

namespace sprite {

class Sprite {
    protected:
    SDL_Rect _sourceRect; //location on sprite sheet to get sprite from
    SDL_Texture* _spriteSheet;
    double _x, _y;

    public:
    Sprite();
    Sprite(graphics::Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, double posX, double posY);
    virtual ~Sprite();
    virtual void update();
    void draw(graphics::Graphics& graphics, int x, int y); //where to draw graphics
};

}

#endif //ZERKOLA_INCLUDE_SPRITE_H_