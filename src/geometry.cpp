#include <geometry.h>

namespace geometry {

//PlotObj
PlotObj::PlotObj(): CG_(0.0,0.0), dir_(0.0,0.0), rad_collision_(0.0) {};

PlotObj::PlotObj(const double& x_start, const double& y_start): CG_(x_start,y_start), dir_(0.0,1.0) {};

PlotObj::~PlotObj() {};

void PlotObj::calc_rad_collision() {
	//Determine radius for collision detection for given shape
	rad_collision_ = 0.0;
	for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
		Eigen::Vector2d v = (*it) - CG_;
		double curr_dist = v.norm();
		if (curr_dist > rad_collision_) {
			rad_collision_ = curr_dist;
		}
	}
	//TODO: Check for collisions at start
	return;
}

void PlotObj::Plot() const {
	//Plot shape boundary object
	namespace plt = matplotlibcpp;
	//Prepare plotting vectors
	std::vector<double> shape_x , shape_y;
	for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
		shape_x.push_back(it->x());
		shape_y.push_back(it->y());
	}
	plt::plot(shape_x,shape_y);
	return;
}
//PlotObj

} // namespace geometry_utils
