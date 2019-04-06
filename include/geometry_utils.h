#ifndef ZERKOLA_INCLUDE_GEOMETRY_UTILS_H_
#define ZERKOLA_INCLUDE_GEOMETRY_UTILS_H_

namespace geometry_utils {

class Point2D {
	//Definition of object position in 2D (x,y)
	private:
	//members
	double x_, y_;

	public:
	Point2D();
	Point2D(double x, double y);
	~Point2D();

	//accessors
	inline double x() { return(x_); };
	double y() { return(y_); };
	inline void set_x(double x) { x_ = x; return; };
	inline void set_y(double y) { y_ = y; return; };

};

class Pose2D {
	//Definition of object pose in 2D (x,y,theta)
	private:
	Point2D pos_;
	double theta_;
	public:

};

}

#endif // ZERKOLA_INCLUDE_GEOMETRY_UTILS_H_
