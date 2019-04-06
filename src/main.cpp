#include "matplotlibcpp.h"
#include <iostream>

namespace plt = matplotlibcpp;
plt::ion();
int main() {
	double dt = 1;
	double t = 0;
	while (t < 1000) {
		plt::plot({5,3,2,4,t});
		//plt::draw();
		plt::show();
		std::cout << t << std::endl;
		t += dt;
	}
}
