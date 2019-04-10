#include <geometry.h>

namespace geometry {

//Point2D
Point2D::Point2D(): point_(0.0,0.0) {};

Point2D::Point2D(const double x, const double y): point_(x,y) {};

Point2D::~Point2D() {};

double Point2D::EucDist(const Point2D& other_point) const {
	Eigen::Vector2d v = point_ - other_point;
	return (v.norm());
}
//Point2D

//PlotObj
PlotObj::PlotObj(): CG_(0.0,0.0), dir_(0.0,0.0), rad_collision(0.0), speed_(0.0) {};

PlotObj::PlotObj(BoundaryShapes shape):Pose2D(50.0,30.0,0.0) {
	shape_boundary.clear();
	switch (shape) {
	case BoundaryShapes::kTank: {
		//Tank Shape
		const double kLength = 3;
		const double kWidth = 2;
		shape_boundary.emplace_back(x_-kWidth/2.0,y_-kLength/2);
		shape_boundary.emplace_back(x_-kWidth/2.0,y_+kLength/2);
		shape_boundary.emplace_back(x_+kWidth/2.0,y_+kLength/2);
		shape_boundary.emplace_back(x_+kWidth/2.0,y_-kLength/2);
		shape_boundary.emplace_back(x_-kWidth/2.0,y_-kLength/2);
		break;
	}
	case BoundaryShapes::kMissile: {
		//Missile Shape
		//TODO
		break;
	}
	default: {
		break;
	}
	}
	//Assert that start and end points are the same
	assert((shape_boundary.front().x() == shape_boundary.back().x()) && (shape_boundary.front().y() == shape_boundary.back().y()));
	//Determine radius for collision detection for given shape
	rad_collision = 0.0;
	for (auto it = shape_boundary.begin(); it != shape_boundary.end(); ++it) {
		double curr_dist = it->EucDist(x_,y_);
		if (curr_dist > rad_collision) {
			rad_collision = curr_dist;
		}
	}
	return;
};

PlotObj::~PlotObj() {};

void PlotObj::Plot() const {
	//Plot shape boundary object
	namespace plt = matplotlibcpp;
	//Prepare plotting vectors
	std::vector<double> shape_x , shape_y;
	for (auto it = shape_boundary.begin(); it != shape_boundary.end(); ++it) {
		shape_x.push_back(it->x());
		shape_y.push_back(it->y());
	}
	plt::plot(shape_x,shape_y);
	return;
}
//PlotObj

} // namespace geometry_utils
