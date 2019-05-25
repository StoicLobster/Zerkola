#ifndef ZERKOLA_INCLUDE_GEOMETRY_H_
#define ZERKOLA_INCLUDE_GEOMETRY_H_

#include <Eigen/Dense>
#include <game_control.h>

namespace geo {

const double RAD_TO_DEG = 180.0/M_PI;

//2D cross product because for some dumb reason Eigen doesnt do that...
inline double CrossProduct2D(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2) { return(v1.x()*v2.y() - v1.y()*v2.x()); } 
/* Returns angle between v1 and v2 in dir (CW or CCW)
 * Angle is in radians
 * Always between -180 and 180 degrees
 */
double AngBetweenVecs(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2, gc::AngularDirections dir=gc::AngularDirections::CCW);
double AngBetweenVecs(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2, gc::AngularDirections dir=gc::AngularDirections::CCW);
bool VectorIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& B0, const Eigen::Vector2d& Bm, double& lambda, Eigen::Vector2d& I);

}

#endif //ZERKOLA_INCLUDE_GEOMETRY_H_