
// Copyright (c) 2019 Travis McCawley. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.
// Credits:
// matplotlib-cpp by Benno Evers: https://github.com/lava/matplotlib-cpp
#include "zerkola.h"
#include <iostream>


int main() {
	//Instantiate Zerkola (game class)
	zerkola::Zerkola zrk;

	//Run game
	zrk.Run();

	return 0;
}
