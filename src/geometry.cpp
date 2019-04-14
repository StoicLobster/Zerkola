#include <geometry.h>

namespace geometry {

//PlotObj
PlotObj::PlotObj(): CG_(0.0,0.0), dir_(0.0,0.0), rad_collision(0.0), long_speed_(0.0), rot_speed_(0.0) {};

PlotObj::PlotObj(const double& x_start, const double& y_start, const BoundaryPoly& shape, const double& long_spd, const double& rot_spd):
	CG_(x_start,y_start),
	dir_(0.0,1.0),
	long_speed_(long_spd),
	rot_speed_(rot_spd)
	{
		polygon_.clear();
		switch (shape) {
		case BoundaryPoly::kTank: {
			//Tank Shape
			const double kLENGTH = 4;
			const double kWIDTH = 2;
			polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()-kLENGTH/2);
			polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()+kLENGTH/2);
			polygon_.emplace_back(CG_.x()+kWIDTH/2.0,CG_.y()+kLENGTH/2);
			polygon_.emplace_back(CG_.x()+kWIDTH/2.0,CG_.y()-kLENGTH/2);
			polygon_.emplace_back(CG_.x()-kWIDTH/2.0,CG_.y()-kLENGTH/2);
			break;
		}
		case BoundaryPoly::kMissile: {
			//Missile Shape
			//TODO
			break;
		}
		default: {
			break;
		}
		}
		//Assert that start and end points are the same
		assert((polygon_.front().x() == polygon_.back().x()) && (polygon_.front().y() == polygon_.back().y()));
		//Determine radius for collision detection for given shape
		rad_collision = 0.0;
		for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
			Eigen::Vector2d v = (*it) - CG_;
			double curr_dist = v.norm();
			if (curr_dist > rad_collision) {
				rad_collision = curr_dist;
			}
		}
		//Check for collisions at start
		return;
	};

PlotObj::~PlotObj() {};

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

void PlotObj::Rot2D(const bool& ccw) {
	Eigen::Rotation2Dd rot = rot_speed_;
	if (!ccw) rot = rot.inverse();
	dir_ = rot*dir_;
	dir_.normalize();
	for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
		Eigen::Vector2d v = (*it) - CG_;
		(*it) = rot*v + CG_;
	}
	return;
}

void PlotObj::Move(const bool& frwd) {
	//Confirm that dir_ is normalized
	dir_.normalize();
	double spd = long_speed_;
	if (!frwd) spd *= -1.0;
	Eigen::Vector2d mvmnt_vec = dir_* spd;
	CG_ += mvmnt_vec;
	for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
		(*it) += mvmnt_vec;
	}
	//TODO: Check if this would violate boundaries and adjust
	return;
}
//PlotObj

} // namespace geometry_utils
