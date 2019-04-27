#ifndef ZERKOLA_INCLUDE_SKYNET_H
#define ZERKOLA_INCLUDE_SKYNET_H
//Includes
#include <tank.h>
#include <missile.h>
#include <string>

namespace skynet{
class SkyNet : public tank::Tank {
    //AI enemy
    public:
    SkyNet();
    SkyNet(const double& x, const double& y,const std::string& color);
    ~SkyNet();
    void Turn(std::list<missile::Missile*>& missiles);
};
}

#endif // ZERKOLA_INCLUDE_SKYNET_H