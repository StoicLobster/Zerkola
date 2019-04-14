#ifndef ZERKOLA_INCLUDE_GEOMETRY_H_
#define ZERKOLA_INCLUDE_GEOMETRY_H_

#include <math.h>
#include <vector>
#include <assert.h>
#include <matplotlibcpp.h>
#include <iostream>
#include <Eigen/Dense>

namespace geometry {

	const double kRAD_TO_DEG = 57.2958;

	enum class BoundaryPoly {
		kTank,
		kMissile
	};

	class PlotObj {
		//A general object to be plotted
		//Object will have CG point and pose (Pose2D) as well as plotted shape (plot_shape) and collision circle radius (rad_col)
		private:
		//members
		Eigen::Vector2d CG_; //Center of object. Object will rotate w.r.t. this point.
		Eigen::Vector2d dir_; //Direction vector which object is pointing (defines forward). Always normalized.
		std::vector<Eigen::Vector2d> polygon_; //Polygon which define plotted shape. Must start and end at the same point
		double rad_collision; //Radius of collision with center of circle at CG_
		double long_speed_; //Longitudinal speed that the object can more each frame
		Eigen::Rotation2Dd rot_speed_; //Rotational speed that the object can more each frame

		public:
		//constructors
		PlotObj();
		PlotObj(const double& x_start, const double& y_start, const BoundaryPoly& shape, const double& long_spd, const double& rot_spd);
		~PlotObj();

		//methods
		void Plot() const; //Plots the current object boundary
		void Rot2D(const bool& ccw); //Rotate object by the rotation speed in the directio indicated
		void Move(const bool& frwd); //Translates object in the corresonding direction by its longitudinal speed
	};

} // namespace geometry

#endif // ZERKOLA_INCLUDE_GEOMETRY_H_
