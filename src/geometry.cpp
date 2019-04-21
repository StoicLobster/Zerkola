#include <geometry.h>

namespace geometry {

//PlotObj
PlotObj::PlotObj(): 
_center(0.0,0.0), 
_dir(0.0,0.0), 
_rad_collision(0.0), 
_color("k"), 
_collision_active(false) 
{};

PlotObj::PlotObj(const double& x_start, const double& y_start, const std::string& color): 
_center(x_start,y_start), 
_dir(0.0,1.0), 
_rad_collision(0.0), 
_color(color), 
_collision_active(false) 
{};

PlotObj::~PlotObj() {};

void PlotObj::_calc_rad_collision() {
	//Determine radius for collision detection for given shape
	_rad_collision = 0.0;
	for (auto it = _polygon.begin(); it != _polygon.end(); ++it) {
		Eigen::Vector2d v = (*it) - _center;
		double curr_dist = v.norm();
		if (curr_dist > _rad_collision) {
			_rad_collision = curr_dist;
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
	for (auto it = _polygon.begin(); it != _polygon.end(); ++it) {
		shape_x.push_back(it->x());
		shape_y.push_back(it->y());
	}
	plt::plot(shape_x,shape_y,_color);
	return;
}

bool PlotObj::CheckCollision(const PlotObj& obj) const {
	double dist = (obj.center() - _center).norm();
	double rad_check = (obj.rad_collision() + _rad_collision);
	//printw("distance: %f, radius #1: %f, radius #2: %f\n",dist,obj.rad_collision(),_rad_collision);
	return ( _collision_active && obj.collision_active() && (dist <= rad_check) );
}
//PlotObj

bool VectorIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& B0, const Eigen::Vector2d& Bm, double& lambda, Eigen::Vector2d& I) {
		//Solve for lambda at intersection of system of two vectors:
		//A = A0 + lambda * Am
		//A = [x_a0, y_a0] + lambda * [x_a, y_a]
		//B = B0 + gamma * Bm
		//B = [x_b0, y_b0] + gamma * [x_b, y_b]
		//If vectors are parallel, returns false (det([Am Bm]) == 0)
		double det_m = Am.x()*Bm.y() - Am.y()*Bm.x();
		if (det_m == 0) {
			//Parallel
			return (false);
		} else {
			//Find intersection
			lambda = (Bm.x()*(A0.y() - B0.y()) - Bm.y()*(A0.x() - B0.x())) / det_m;
			I = A0 + lambda * Am;
			return (true);
		}
	}

} // namespace geometry_utils
