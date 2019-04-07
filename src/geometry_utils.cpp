#include <geometry_utils.h>

namespace geometry_utils {

Point2D::Point2D(): x_(0.0),y_(0.0) {};

Point2D::Point2D(double x, double y): x_(x),y_(y) {};

Point2D::~Point2D() {};

Pose2D::Pose2D(): theta_(0.0) {};

Pose2D::Pose2D(double x, double y, double theta): Point2D(x,y), theta_(theta) {};

} // namespace geometry_utils
