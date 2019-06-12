#ifndef ZERKOLA_INCLUDE_R2D2_H_
#define ZERKOLA_INCLUDE_R2D2_H_

#include <tank.h>
#include <graphics.h>
#include <game_control.h>
#include <iostream>

namespace r2d2 {

/* Distance between tank and boarder by which point navigation maneuvers will begin */
const double NAVIGATION_THRESHOLD = 100;

/* ManeuverType
 * 0: MANEUVER_TYPE_NONE
 * 1: NAVIGATION
 * 2: EVASIVE
 * 3: AGGRESSIVE
 * 4: NUM_MANEUVER_TYPE
 */
typedef enum ManeuverType {
    mtNONE,
    mtINIT,
    mtNAVIGATION,
    mtEVASIVE,
    mtAGGRESSIVE,
    mtNUM
} ManeuverType;

/* Easy computer player */
class R2D2 : public tank::Tank {

/*=== START PRIVATE ===*/
private:
    /* Current maneuver goal of R2D2 */
    ManeuverType _currentManeuver;

    /* Previous commands */
    gc::LinearDirection _translate_body_cmnd_prev;
    gc::AngularDirection _rotate_body_cmnd_prev;
    gc::AngularDirection _rotate_turret_cmnd_prev;

    /* Print state of R2D2 tank */
    virtual void _printState() const;

    /* R2D2 turn. Policy based strategy */
    virtual void _turn();

    /* Returns TRUE if tank is close to the boarder */
    bool _boarderProximityCheck() const;

    /* Returns TRUE if tank is in danger of a missile strike */
    bool _dangerCheck() const;

    /* Moves the tank away from boarders */
    void _navigateManeuver();

    /* Moves the tank away from danger */
    void _evasiveManeuver();

    /* Orients the tank towrads _enemyTarget in order to fire */
    void _aggressiveManeuver();

    /* Returns TRUE if any missile would collide with tank on current trajectory */
    bool _directMissileHitCheck() const;
/*=== END PRIVATE ===*/

/*=== START PUBLIC ===*/
public:
    R2D2();
    ~R2D2();
    R2D2(graphics::Graphics* graphics_ptr, gc::PlayerColor player_color, std::list<missile::Missile*>* missiles_ptr);
/*=== END PUBLIC ===*/

}; //class R2D2

} //namespace r2d2

// Overlaod ostream operator for enum
std::ostream& operator<<(std::ostream& os, const r2d2::ManeuverType& inpt);

#endif //ZERKOLA_INCLUDE_R2D2_H_