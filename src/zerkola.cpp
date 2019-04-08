#include <zerkola.h>

namespace zerkola {

Zerkola::Zerkola(): kNorthLimit_(100.0), kEastLimit_(100.0), kSouthLimit_(0.0), kWestLimit_(0.0), primary_fig_num_(0) {};

Zerkola::~Zerkola() {};

void Zerkola::init_plot() {
	primary_fig_num_ = plt::figure();
	plt::axis("off");
	return;
}

void Zerkola::Run() {
	init_plot();
	double t = 0;
	while (true) {
		//Clear plot
		plt::cla();
		plt::axis("off");
		//Update plot
		plt::plot({5,3,2,4,t});
		//plt::draw();
		plt::pause(0.1);
		t += 10;
	}

	//
	return;
}

} // namespace zerkola

