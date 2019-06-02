#ifndef ZERKOLA_INCLUDE_GEOMETRY_H_
#define ZERKOLA_INCLUDE_GEOMETRY_H_
//Includes
#include <Eigen/Dense>
#include <game_control.h>

/* Utilities for geometry calculations */
namespace geo {

/* Convert radians to degrees */
const double RAD_TO_DEG = 180.0/M_PI;

/* Cast 3D Eigen vector to 2D (d)*/
inline Eigen::Vector2d Cast3D2Dd(Eigen::Vector3d vec_3) { return(Eigen::Vector2d(vec_3.x(),vec_3.y())); };

/* Cast 3D Eigen vector to 2D (i)*/
inline Eigen::Vector2i Cast3D2Dd(Eigen::Vector3i vec_3) { return(Eigen::Vector2i(vec_3.x(),vec_3.y())); };

/* CrossProduct2D()
 * 2D cross product because for some dumb reason Eigen doesnt do that...
 */
inline double CrossProduct2D(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2) { return(v1.x()*v2.y() - v1.y()*v2.x()); };

/* CrossProduct2D()
 * 2D cross product because for some dumb reason Eigen doesnt do that...
 * 3D vector version
 */
inline double CrossProduct2D(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2) { return(v1.x()*v2.y() - v1.y()*v2.x()); };

/* AngBetweenVecs()
 * Returns angle between v1 and v2 in dir (CW or CCW)
 * Angle is in radians
 * Always between -180 and 180 degrees
 */
double AngBetweenVecs(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2, gc::AngularDirections dir=gc::AngularDirections::CCW);

/* AngBetweenVecs()
 * Returns angle between v1 and v2 in dir (CW or CCW)
 * Angle is in radians
 * Always between -180 and 180 degrees
 */
double AngBetweenVecs(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2, gc::AngularDirections dir=gc::AngularDirections::CCW);

/* LineLineIntersection()
 * Returns intersection point between two lines
 * Each line is defined by a point A0 / B0 and a direction vector Am / Bm
 * Return lambda, parametric scalar from A0 / B0 to the intersection point I and boolean if intersection occurs (non-parallel)
 */
bool LineLineIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& B0, const Eigen::Vector2d& Bm, double& lambda, Eigen::Vector2d& I);

/* MinDistLinePoint()
 * Returns minimum distance between a line r = A0 + lambda*Am, and a point P0
 * Returns minimum distance (lambda) and intersection point on r (I)
 */
void MinDistLinePoint(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& P0, double& lambda, Eigen::Vector2d& I);

/* LineCircleIntersection()
 * Returns true if line intersects with circle
 * Line is defined by a point A0 and a direction vector Am
 * Circle is defined by point C0 and a radius r
 * Returns minimum distance (lambda) to closest point on circle (I)
 */
bool LineCircleIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& C0, double r, double& lambda, Eigen::Vector2d& I, bool verbose=false);

} //namespace geo

#endif //ZERKOLA_INCLUDE_GEOMETRY_H_