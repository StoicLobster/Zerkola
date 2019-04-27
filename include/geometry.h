#ifndef ZERKOLA_INCLUDE_GEOMETRY_H_
#define ZERKOLA_INCLUDE_GEOMETRY_H_
//Includes
#include <math.h>
#include <vector>
#include <assert.h>
#include <matplotlibcpp.h>
#include <iostream>
#include <Eigen/Dense>
#include <string>
#include <ncurses.h>
#include <map>

namespace geometry {

	//constants
	//constants

	class PlotObj {
		//A general object to be plotted
		//Object will have CG point and pose (Pose2D) as well as plotted shape (plot_shape) and collision circle radius (rad_col)
		protected:
		//members
		Eigen::Vector2d _center; //Center of object. Object will rotate w.r.t. this point.
		Eigen::Vector2d _dir; //Direction vector which object is pointing (defines forward). Always normalized.
		std::vector<Eigen::Vector2d> _polygon; //Polygon which define plotted shape. Must start and end at the same point
		double _rad_collision; //Radius of collision with center of circle at CG_
		std::string _color; //color of object
		bool _collision_active; //Indicates that object is active for collision detection
		//methods
		void _calc_rad_collision(); //Calculates radius of collision for given polygon_

		public:
		//constructors
		PlotObj();
		PlotObj(const double& x_start, const double& y_start, const std::string& color="k");
		~PlotObj();
		void UpdatePolygon(const std::vector<Eigen::Vector2d>& poly);

		//methods
		void Plot(int& zord) const; //Plots the current object boundary with given starting zorder
		inline Eigen::Vector2d center() const { return(_center); };
		inline double rad_collision() const { return(_rad_collision); };
		inline double collision_active() const { return(_collision_active); }; 
		bool CheckCollision(const PlotObj& obj) const; //Checks if this object collides with given object
		void Translate(const Eigen::Vector2d& mvmnt_vec); //Translates object / polygon by given movement vector
		void Rotate(const Eigen::Rotation2Dd& rot, const Eigen::Vector2d& cntr); //Rotate object / polygon about center by rotation
	};

	bool VectorIntersection(const Eigen::Vector2d& A0, const Eigen::Vector2d& Am, const Eigen::Vector2d& B0, const Eigen::Vector2d& Bm, double& lambda, Eigen::Vector2d& I);

} // namespace geometry

#endif // ZERKOLA_INCLUDE_GEOMETRY_H_
