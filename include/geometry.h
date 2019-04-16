#ifndef ZERKOLA_INCLUDE_GEOMETRY_H_
#define ZERKOLA_INCLUDE_GEOMETRY_H_

#include <math.h>
#include <vector>
#include <assert.h>
#include <matplotlibcpp.h>
#include <iostream>
#include <Eigen/Dense>
#include <string>

namespace geometry {

	//constants
	const double kRAD_TO_DEG = 57.2958;
	typedef enum LimCollision {
		None,
		NorthCollision,
		EastCollision,
		SouthCollision,
		WestCollision
	} LimCollision;
	//constants

	class PlotObj {
		//A general object to be plotted
		//Object will have CG point and pose (Pose2D) as well as plotted shape (plot_shape) and collision circle radius (rad_col)
		protected:
		//members
		Eigen::Vector2d CG_; //Center of object. Object will rotate w.r.t. this point.
		Eigen::Vector2d dir_; //Direction vector which object is pointing (defines forward). Always normalized.
		std::vector<Eigen::Vector2d> polygon_; //Polygon which define plotted shape. Must start and end at the same point
		double rad_collision_; //Radius of collision with center of circle at CG_
		std::string color_; //color of object
		//methods
		void calc_rad_collision(); //Calculates radius of collision for given polygon_

		public:
		//constructors
		PlotObj();
		PlotObj(const double& x_start, const double& y_start,const std::string& color="k");
		~PlotObj();

		//methods
		void Plot() const; //Plots the current object boundary
		bool CheckBoundaryCollision(const double& NorthLimit, const double& EastLimit, const double& SouthLimit, const double& WestLimit, LimCollision& type_col) const; //Checks if PlotObj collided with boundary
	};

	bool VectorIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& B0, const Eigen::Vector2d& Bm, double& lambda, double& xi, double& yi) {
		//Solve for lambda at intersection of system of two vectors:
		//A = A0 + lambda * Am
		//A = [x_a0, y_a0] + lambda * [x_a, y_a]
		//B = B0 + gamma * Bm
		//B = [x_b0, y_b0] + gamma * [x_b, y_b]
		//If vectors are parallel, returns false (det([Am Bm]) == 0)
		double det_m = Am.x()*Bm.y() - Am.y()*Bm.x();
		if (det_m == 0) {
			//Parallel
			return (false);
		} else {
			//Find intersection
			lambda = Bm.x()*(A0.y() - B0.y()) - Bm.y()*(A0.x() - B0.x()) / det_m;
			//TODO: calculate xi / yi
			return true;
		}
	}

} // namespace geometry

#endif // ZERKOLA_INCLUDE_GEOMETRY_H_
