#include <geometry.h>

namespace geometry {

//Point2D
Point2D::Point2D(): x_(0.0),y_(0.0) {};

Point2D::Point2D(const double x, const double y): x_(x),y_(y) {};

Point2D::~Point2D() {};
//Point2D

//Pose2D
Pose2D::Pose2D(): theta_(0.0) {};

Pose2D::Pose2D(const double x, const double y, const double theta): Point2D(x,y), theta_(theta) {};

Pose2D::~Pose2D() {};
//Pose2D

} // namespace geometry_utils
