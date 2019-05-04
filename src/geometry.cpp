#include <geometry.h>
#include <math.h>

namespace geo {

double AngBetweenVecs(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2, gc::AngularDirections dir) {
    /* tan(theta) = ||u cross v|| / (u dot v)
     * Use atan2 to avoid divide by zero errors when u and v are perpindicular
     */
    double theta = atan2(CrossProduct2D(v1,v2), v1.dot(v2));
    if (dir == gc::AngularDirections::CW) theta *= -1;
    return(theta);
}

double AngBetweenVecs(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2, gc::AngularDirections dir) {
    /* tan(theta) = ||u cross v|| / (u dot v)
     * Use atan2 to avoid divide by zero errors when u and v are perpindicular
     */
    double theta = atan2(v1.cross(v2).norm(), v1.dot(v2));
    if (dir == gc::AngularDirections::CW) theta *= -1;
    return(theta);
}

}