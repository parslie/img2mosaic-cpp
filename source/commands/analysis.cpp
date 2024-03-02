#include "analysis.hpp"
#include "../data/palette.hpp"

#include <array>
#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = filesystem;

static bool isImageFile(const fs::path path)
{
	string EXTS[] = { ".png", ".jpg", ".jpeg", ".webp" };
	for (string ext : EXTS)
		if (ext == path.extension())
			return true;
	return false;
}

static vector<fs::path> getUnanalyzedPaths(const string directoryPath, const bool recursive, const Palette &palette)
{
	vector<fs::path> paths;

	if (recursive)
	{
		for (auto &entry : fs::recursive_directory_iterator(directoryPath))
			if (isImageFile(entry.path()) && !paletteContains(palette, entry.path().string()))
				paths.push_back(entry.path());
	}
	else
	{
		for (auto &entry : fs::directory_iterator(directoryPath))
			if (isImageFile(entry.path()) && !paletteContains(palette, entry.path().string()))
				paths.push_back(entry.path());
	}

	return paths;
}

void analysis::run(const Arguments &arguments)
{
	// 1. Initialize palette
	Palette palette = loadPalette(arguments.profile);

	// 2. Get paths that haven't been analyzed
	vector<fs::path> paths = getUnanalyzedPaths(arguments.analysisArgs.directoryPath, arguments.analysisArgs.recursive, palette);
	cout << "Got " << paths.size() << " unanalyzed images." << endl;

	// 3. Iterate over each path
	//	  a. Get average color
	//	  b. Add color to palette

	// 4. Save palette
	savePalette(arguments.profile, palette);
}