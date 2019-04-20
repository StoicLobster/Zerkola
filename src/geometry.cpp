#include <geometry.h>

namespace geometry {

//PlotObj
PlotObj::PlotObj(): CG_(0.0,0.0), dir_(0.0,0.0), rad_collision_(0.0), color_("k") {};

PlotObj::PlotObj(const double& x_start, const double& y_start, const std::string& color): CG_(x_start,y_start), dir_(0.0,1.0), color_(color) {};

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
	plt::plot(shape_x,shape_y,color_);
	return;
}

bool PlotObj::CheckBoundaryCollision(const double& NorthLimit, const double& EastLimit, const double& SouthLimit, const double& WestLimit, LimCollision& type_col) const {
	double TL, BL, LL, RL;
	bool outpt = false;
	type_col = LimCollision::None;
	TL = CG_.y() + rad_collision_;
	BL = CG_.y() - rad_collision_;
	LL = CG_.x() - rad_collision_;
	RL = CG_.x() + rad_collision_;

	if (TL >= NorthLimit) {
		outpt = true;
		type_col = LimCollision::NorthCollision;
	} else if (BL <= SouthLimit) {
		outpt = true;
		type_col = LimCollision::SouthCollision;
	} else if (LL <= WestLimit) {
		outpt = true;
		type_col = LimCollision::WestCollision;
	} else if (RL >= EastLimit) {
		outpt = true;
		type_col = LimCollision::EastCollision;
	}

	return (outpt);
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
