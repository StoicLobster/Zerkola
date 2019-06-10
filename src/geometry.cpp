#include <geometry.h>
#include <math.h>
#include <iostream>

namespace geo {

template <class T>
double AngBetweenVecs(const T& v1, const T& v2, gc::AngularDirection dir) {
    /* tan(theta) = (u cross v) / (u dot v)
     * Use atan2 to avoid divide by zero errors when u and v are perpindicular
     */
    double theta = atan2(CrossProduct2D(v1,v2), v1.dot(v2));
    if (dir == gc::AngularDirection::CW) theta *= -1;
    return(theta);
}

// double AngBetweenVecs(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2, gc::AngularDirection dir) {
//     /* tan(theta) = (u cross v) / (u dot v)
//      * Use atan2 to avoid divide by zero errors when u and v are perpindicular
//      */
//     double theta = atan2(CrossProduct2D(v1,v2), v1.dot(v2));
//     if (dir == gc::AngularDirection::CW) theta *= -1;
//     return(theta);
// }

bool LineLineIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& B0, const Eigen::Vector2d& Bm, double& lambda, Eigen::Vector2d& I) {
    //Solve for lambda at intersection of system of two vectors:
    //A = A0 + lambda * Am
    //A = [x_a0, y_a0] + lambda * [x_a, y_a]
    //B = B0 + gamma * Bm
    //B = [x_b0, y_b0] + gamma * [x_b, y_b]
    //If vectors are parallel, returns false (det([Am Bm]) == 0)
    //Ensure direction vectors are normalized
    Eigen::Vector2d Am_norm, Bm_norm;
    Am_norm = Am.normalized();
    Bm_norm = Bm.normalized();
    double det_m = CrossProduct2D(Am_norm,Bm_norm);
    if (det_m == 0) {
        //Parallel
        return (false);
    } else {
        //Find intersection
        lambda = (Bm_norm.x()*(A0.y() - B0.y()) - Bm_norm.y()*(A0.x() - B0.x())) / det_m;
        I = A0 + lambda * Am_norm;
        return (true);
    }
}

void MinDistLinePoint(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& P0, double& lambda, Eigen::Vector2d& I) {
    // Equation of line: r = A0 + alpha*Am
    // Equation of line connecting point and orthogonal to r: u = P0 + lambda*<Amy -Amx>
    // Solution: lambda = (Amx*(P0y - A0y) + Amy*(A0x - P0x)) / (Amx^2 + Amy^2)
    // Must normalize Am in order for lambda to be distance
    Eigen::Vector2d Am_norm = Am.normalized();
    lambda = (Am_norm.x()*(P0.y() - A0.y()) + Am_norm.y()*(A0.x() - P0.x())) / (Am_norm.x()*Am_norm.x() + Am_norm.y()*Am_norm.y());
    I.x() = P0.x() + lambda*Am_norm.y();
    I.y() = P0.y() - lambda*Am_norm.x();
    return;
}

bool LineCircleIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& C0, double r, double& lambda, Eigen::Vector2d& I, bool verbose) {
    if (verbose) std::cout << "LineCircleIntersection()" << std::endl;
    // Must normalize Am in order for lambda to be distance
    Eigen::Vector2d Am_norm = Am.normalized();
    if (verbose) std::cout << "Am_norm: (" << Am_norm.x() << ", " << Am_norm.y() << ")" << std::endl;
    //Get minimum distance from line to point
    double min_dist = r + 1;
    Eigen::Vector2d K;
    MinDistLinePoint(A0,Am_norm,C0,min_dist,K);
    min_dist = abs(min_dist);
    if (verbose) std::cout << "min_dist: " << min_dist << ", K: (" << K.x() << ", " << K.y() << ")" << std::endl;
    if (min_dist > r) return(false);
    //Find point on circle that intersects with line <A0, Am> and is closest to A0
    //Use parametric equation for circle and solve system of equations (requires quadratic formula)
    double a, b, c, qA, q_rad, qB, r1, r2;
    a = pow(Am_norm.x(),2.0) + pow(Am_norm.y(),2.0);
    b = 2*(Am_norm.x()*(A0.x()-C0.x()) + Am_norm.y()*(A0.y()-C0.y()));
    c = pow((A0.x()-C0.x()),2.0) + pow((A0.y()-C0.y()),2.0) - pow(r,2.0);
    if (verbose) std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
    assert(a != 0);
    qA = -b/(2*a);
    q_rad = b*b - 4*a*c;
    assert(q_rad >= 0);
    qB = sqrt(q_rad)/(2*a);
    r1 = qA + qB;
    r2 = qA - qB;
    if (verbose) std::cout << "qA: " << qA << ", q_rad: " << q_rad << ", qB: " << qB << ", r1: " << r1 << ", r2: " << r2 << std::endl;
    //Select root which is closest to A0 (smaller)
    if (r1 < r2) lambda = r1;
    else lambda = r2;
    //Calculate intersection point
    I = A0 + lambda*Am_norm;
    return(true);
}

void BoundaryMinDist(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, Eigen::Vector2d& intersect_pt, double& intersect_dist) {   
    //Init with max possible distance 
    intersect_dist = sqrt( pow((gc::BOARD_PHYS_TOP-gc::BOARD_PHYS_BOTTOM),2) + pow((gc::BOARD_PHYS_RIGHT-gc::BOARD_PHYS_LEFT),2) ); 
    for (const auto& bdry : gc::BOARD_PHYS_BOUNDARIES) {
        Eigen::Vector2d I, B0, Bm;
        B0 = bdry.first.cast<double>();
        Bm = bdry.second.cast<double>();
        double lambda;
        if (LineLineIntersection(A0,Am,B0,Bm,lambda,I)) {
            //non-parallel
            if ((lambda > 0) && (lambda < intersect_dist)) {
                intersect_dist = lambda;
                intersect_pt = I;
            }
        }
    }
    return;
}

}