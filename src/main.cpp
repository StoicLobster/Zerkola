/* Copyright (c) 2019 Travis McCawley. All rights reserved.
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

//Includes
#include <iostream>
#include <zerkola.h>
#include <SDL2/SDL.h>


int main(int argc, const char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	// //Test geometry
	// Eigen::Vector2d A0, Am, C0, I;
	// double r, lambda;
	// A0.x() = 0;
	// A0.y() = 0;
	// Am.x() = 1;
	// Am.y() = 4;
	// C0.x() = 4;
	// C0.y() = 8;
	// r = 1;
	// if (geo::LineCircleIntersection(A0, Am, C0, r, lambda, I)) {
	// 	std::cout << "Intersection Found!" << std::endl;
	// 	std::cout << "lambda: " << lambda << ", I: (" << I.x() << ", " << I.y() << ")" << std::endl;
	// } else {
	// 	std::cout << "No Intersection!" << std::endl;
	// }
	//Instantiate Zerkola (game class)
	zerkola::Zerkola zrk;

	return 0;
}
