#include <skynet.h>

namespace skynet {
	SkyNet::SkyNet() {};

	SkyNet::SkyNet(const double& x, const double& y,const std::string& color): Tank(x, y, color) {};

	SkyNet::~SkyNet() {};

	void SkyNet::Turn(std::list<missile::Missile*>& missiles) {
		return;
	}
}
