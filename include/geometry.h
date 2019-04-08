#ifndef ZERKOLA_INCLUDE_GEOMETRY_H_
#define ZERKOLA_INCLUDE_GEOMETRY_H_

#include <vector>

namespace geometry {

enum class BoundaryShapes {
	kTank,
	kMissile
};

class Point2D {
	//Definition of object position in 2D (x,y)
	protected:
	//members
	double x_, y_;

	public:
	//constructors
	Point2D();
	Point2D(const double x, const double y);
	~Point2D();

	//accessors
	inline double x() const { return(x_); };
	inline double y() const { return(y_); };
	inline void set_x(const double x) { x_ = x; return; };
	inline void set_y(const double y) { y_ = y; return; };

};

class Pose2D : protected Point2D {
	//Definition of object pose in 2D (x,y,theta)
	protected:
	//members
	double theta_;

	public:
	//constructors
	Pose2D();
	Pose2D(double x, double y, double theta);
	~Pose2D();

	//accessors
	inline double theta() const { return(theta_); };
	inline void set_theta(const double theta) { theta_ = theta; };

};

class PlotObj : protected Pose2D {
	//A general object to be plotted
	//Object will have CG point and pose (Pose2D) as well as plotted shape (plot_shape) and collision circle radius (rad_col)
	private:
	//members
	std::vector<Point2D> plot_shape; //Vector of points which define plotted shape. Must start and end at the same point
	double rad_col; //Radius of collision with center of circle at (x_,y_)

	public:
	PlotObj();
	PlotObj(BoundaryShapes shape);
	~PlotObj();
};

} // namespace geometry

#endif // ZERKOLA_INCLUDE_GEOMETRY_H_
