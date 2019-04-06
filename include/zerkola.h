#ifndef ZERKOLA_INCLUDE_ZERKOLA_H_
#define ZERKOLA_INCLUDE_ZERKOLA_H_
#include <matplotlibcpp.h>
#include <geometry_utils.h>

namespace plt = matplotlibcpp;
namespace zerkola {

class Zerkola {

	private:
	//members
	const double NL_, EL_, SL_, WL_; //Limits of game board in North, South, East, West
	long primary_fig_num_; //Number to access primary plot figure
	geometry_utils::Point2D
	//methods
	void init_plot();

	public:
	Zerkola();
	~Zerkola();
	void Run();
};

} // namespace zerkola
#endif // ZERKOLA_INCLUDE_ZERKOLA_H_
