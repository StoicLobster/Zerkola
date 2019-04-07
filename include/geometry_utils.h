#ifndef ZERKOLA_INCLUDE_GEOMETRY_UTILS_H_
#define ZERKOLA_INCLUDE_GEOMETRY_UTILS_H_

namespace geometry_utils {

class Point2D {
	//Definition of object position in 2D (x,y)
	protected:
	//members
	double x_, y_;

	public:
	//constructors
	Point2D();
	Point2D(double x, double y);
	~Point2D();

	//accessors
	inline double x() const { return(x_); };
	inline double y() const { return(y_); };
	inline void set_x(double x) { x_ = x; return; };
	inline void set_y(double y) { y_ = y; return; };

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
	inline void set_theta(double theta) { theta_ = theta; };


};

}

#endif // ZERKOLA_INCLUDE_GEOMETRY_UTILS_H_
