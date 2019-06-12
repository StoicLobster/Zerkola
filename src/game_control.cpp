#include <game_control.h>

std::ostream& operator<<(std::ostream& os, const gc::PlayerColor& inpt) {
	switch (inpt) {
		case gc::PlayerColor::BLUE:
			os << "BLUE";
			break;
		case gc::PlayerColor::RED:
			os << "RED";
			break;
		default:
			os << "ERROR";
			break;
	}
    return os;
}

std::ostream& operator<<(std::ostream& os, const gc::LinearDirection& inpt) {
	switch (inpt) {
		case gc::LinearDirection::FORWARD:
			os << "FORWARD";
			break;
		case gc::LinearDirection::BACKWARD:
			os << "BACKWARD";
			break;
		case gc::LinearDirection::LINEAR_DIRECTION_NONE:
			os << "NONE";
			break;
		default:
			os << "ERROR";
			break;
	}
    return os;
}

std::ostream& operator<<(std::ostream& os, const gc::AngularDirection& inpt) {
	switch (inpt) {
		case gc::AngularDirection::CCW:
			os << "CCW";
			break;
		case gc::AngularDirection::CW:
			os << "CW";
			break;
		case gc::AngularDirection::ANGULAR_DIRECTION_NONE:
			os << "NONE";
			break;
		default:
			os << "ERROR";
			break;
	}
    return os;
}

std::ostream& operator<<(std::ostream& os, const gc::TankMotionState& inpt) {
	switch (inpt) {
		case gc::TankMotionState::STATIONARY:
			os << "STATIONARY";
			break;
		case gc::TankMotionState::PROPULSION_FORWARD:
			os << "PROPULSION_FORWARD";
			break;
		case gc::TankMotionState::BRAKING:
			os << "BRAKING";
			break;
		case gc::TankMotionState::SHIFTING:
			os << "SHIFTING";
			break;
		case gc::TankMotionState::REVERSE:
			os << "REVERSE";
			break;
		default:
			os << "ERROR";
			break;
	}
    return os;
}