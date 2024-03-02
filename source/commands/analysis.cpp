#include "analysis.hpp"

#include "../data/palette.hpp"

void analysis::run(const Arguments &arguments)
{
	// 1. Initialize palette
	Palette palette = loadPalette(arguments.profile);
	//printPalette(palette);

	// 2. Get paths that haven't been analyzed

	// 3. Iterate over each path
	//	  a. Get average color
	//	  b. Add color to palette

	// 4. Save palette
	savePalette(arguments.profile, palette);
}