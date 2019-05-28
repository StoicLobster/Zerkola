#ifndef ZERKOLA_INCLUDE_HUMAN_PLAYER_H_
#define ZERKOLA_INCLUDE_HUMAN_PLAYER_H_
//Includes
#include <tank.h>

namespace human_player {

/* Human player class */
class HumanPlayer : public tank::Tank {

/*=== START PRIVATE ===*/
private:
    /* Pointer to inputs */
    input::Input* _input_ptr;

    /* Human player turn. Reads inputs and distributes commands */
    void _turn(double dt_ms);
/*=== END PRIVATE ===*/

/*=== START PUBLIC ===*/
public:
    HumanPlayer();
    ~HumanPlayer();
    HumanPlayer(graphics::Graphics* graphics_ptr, 
        std::list<missile::Missile*>* missiles_ptr, 
        input::Input* input_ptr);

/*=== END PUBLIC ===*/

}; //class HumanPlayer

} //namespace human_player

#endif //ZERKOLA_INCLUDE_HUMAN_PLAYER_H_