#include <geometry.h>

namespace geometry {

//Point2D
Point2D::Point2D(): point_(0.0,0.0) {};

Point2D::Point2D(const double x, const double y): point_(x,y) {};

Point2D::~Point2D() {};

double Point2D::EucDist(const Point2D& other_point) const {
	Eigen::Vector2d v(this->x() - other_point.x(), this->y() - other_point.y());
	return (v.norm());
}
//Point2D

//PlotObj
PlotObj::PlotObj(): CG_(0.0,0.0), dir_(0.0,0.0), rad_collision(0.0), speed_(0.0) {};

PlotObj::PlotObj(const double& x_start, const double& y_start, const BoundaryPoly& shape, const double& spd):
	CG_(x_start,y_start),
	dir_(0.0,0.0),
	speed_(spd)
	{
		

		polygon_.clear();
		switch (shape) {
		case BoundaryPoly::kTank: {
			//Tank Shape
			const double kLength = 3;
			const double kWidth = 2;
			polygon_.emplace_back(CG_.x()-kWidth/2.0,CG_.y()-kLength/2);
			polygon_.emplace_back(CG_.x()-kWidth/2.0,CG_.y()+kLength/2);
			polygon_.emplace_back(CG_.x()+kWidth/2.0,CG_.y()+kLength/2);
			polygon_.emplace_back(CG_.x()+kWidth/2.0,CG_.y()-kLength/2);
			polygon_.emplace_back(CG_.x()-kWidth/2.0,CG_.y()-kLength/2);
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
			double curr_dist = it->EucDist(CG_.x(),CG_.y());
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

void PlotObj::Move() {
	CG_.set_y(CG_.y() + speed_);
	for (auto it = polygon_.begin(); it != polygon_.end(); ++it) {
		it->set_y(it->y() + speed_);
	}
}
//PlotObj

} // namespace geometry_utils
