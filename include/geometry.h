#ifndef ZERKOLA_INCLUDE_GEOMETRY_H_
#define ZERKOLA_INCLUDE_GEOMETRY_H_

#include <math.h>
#include <vector>
#include <assert.h>
#include <matplotlibcpp.h>
#include <iostream>
#include <Eigen/Dense>

namespace geometry {

enum class BoundaryPoly {
	kTank,
	kMissile
};

class Point2D {
	//Definition of object position in 2D (x,y)
	protected:
	//members
	Eigen::Vector2d point_;

	public:
	//constructors
	Point2D();
	Point2D(const double x, const double y);
	~Point2D();

	//accessors
	inline double x() const { return( point_(0) ); };
	inline double y() const { return( point_(1) ); };
	inline void set_x(const double x) { point_(0) = x; return; };
	inline void set_y(const double y) { point_(1) = y; return; };

	//methods
	double EucDist(const Point2D& other_point) const;
	inline double EucDist(const double& x, const double& y) const { return( sqrt( powf((this->x() - x),2.0) + powf((this->y() - y),2.0)) ); };
	inline void Print() const { std::cout << "(" << this->x() << "," << this->y() << ")" << std::endl; return; }

};

class PlotObj {
	//A general object to be plotted
	//Object will have CG point and pose (Pose2D) as well as plotted shape (plot_shape) and collision circle radius (rad_col)
	private:
	//members
	Point2D CG_; //Center of object. Object will rotate w.r.t. this point.
	Point2D dir_; //Direction vector which object is pointing (defines forward).
	std::vector<Point2D> polygon_; //Polygon which define plotted shape. Must start and end at the same point
	double rad_collision; //Radius of collision with center of circle at CG_
	double speed_; //Speed that object can more each frame

	public:
	//constructors
	PlotObj();
	PlotObj(const double& x_start, const double& y_start, const BoundaryPoly& shape, const double& spd);
	~PlotObj();

	//methods
	void Plot() const; //Plots the current object boundary
	void Rot2D(const double& theta); //Rotate object by theta [rad] about CG
	void Move(); //Translates object in the direction it is pointing by its speed
};

} // namespace geometry

#endif // ZERKOLA_INCLUDE_GEOMETRY_H_
