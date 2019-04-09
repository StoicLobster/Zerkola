#include <zerkola.h>

namespace zerkola {

Zerkola::Zerkola(): kNorthLimit_(100.0), kEastLimit_(100.0), kSouthLimit_(0.0), kWestLimit_(0.0),
		kGameBoardBoundaryX_{kNorthLimit_,kNorthLimit_,kSouthLimit_,kSouthLimit_,kNorthLimit_},
		kGameBoardBoundaryY_{kWestLimit_,kEastLimit_,kEastLimit_,kWestLimit_,kWestLimit_},
		primary_fig_num_(0){};

Zerkola::~Zerkola() {};

void Zerkola::init_plot() {
	primary_fig_num_ = plt::figure();
	plt::axis("off");
	return;
}

void Zerkola::Run() {
	//Initialize plot
	init_plot();
	//Instantiate game pieces
	geometry::PlotObj tank_player(geometry::BoundaryShapes::kTank);
	while (true) {
		//Clear plot
		plt::cla();
		plt::axis("off");
		plt::plot(kGameBoardBoundaryX_,kGameBoardBoundaryY_);
		plt::tight_layout();
		//Update plot
		//plt::plot({5,3,2,4,t});
		tank_player.Plot();
		//plt::draw();
		plt::pause(0.1);
	}

	return;
}

} // namespace zerkola

