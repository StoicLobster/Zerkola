#include <geometry.h>
#include <math.h>

namespace geo {

double AngBetweenVecs(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2, gc::AngularDirections dir) {
    /* tan(theta) = (u cross v) / (u dot v)
     * Use atan2 to avoid divide by zero errors when u and v are perpindicular
     */
    double theta = atan2(CrossProduct2D(v1,v2), v1.dot(v2));
    if (dir == gc::AngularDirections::CW) theta *= -1;
    return(theta);
}

double AngBetweenVecs(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2, gc::AngularDirections dir) {
    /* tan(theta) = (u cross v) / (u dot v)
     * Use atan2 to avoid divide by zero errors when u and v are perpindicular
     */
    double theta = atan2(CrossProduct2D(v1,v2), v1.dot(v2));
    if (dir == gc::AngularDirections::CW) theta *= -1;
    return(theta);
}

bool VectorIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& B0, const Eigen::Vector2d& Bm, double& lambda, Eigen::Vector2d& I) {
    //Solve for lambda at intersection of system of two vectors:
    //A = A0 + lambda * Am
    //A = [x_a0, y_a0] + lambda * [x_a, y_a]
    //B = B0 + gamma * Bm
    //B = [x_b0, y_b0] + gamma * [x_b, y_b]
    //If vectors are parallel, returns false (det([Am Bm]) == 0)
    double det_m = CrossProduct2D(Am,Bm);
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

}