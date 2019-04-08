#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_
#include <matplotlibcpp.h>
#include <geometry.h>

namespace plt = matplotlibcpp;
namespace zerkola {

class Zerkola {

	private:
	//members
	const double kNorthLimit_, kEastLimit_, kSouthLimit_, kWestLimit_; //Limits of game board in North, South, East, West
	long primary_fig_num_; //Number to access primary plot figure
	geometry::Pose2D x;
	//methods
	void init_plot();

	public:
	Zerkola();
	~Zerkola();
	void Run();
};

} // namespace zerkola
#endif // ZERKOLA_INCLUDE_ZERKOLA_H_
