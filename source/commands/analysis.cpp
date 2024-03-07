#include "analysis.hpp"
#include "../data/palette.hpp"
#include "../utils/image.hpp"

#include <array>
#include <iostream>
#include <filesystem>
#include <vector>

#include <opencv2/opencv.hpp>

using namespace std;
namespace fs = filesystem;

static bool isImgFile(const fs::path path)
{
	string EXTS[] = { ".png", ".jpg", ".jpeg", ".webp" };
	for (string ext : EXTS)
		if (ext == path.extension())
			return true;
	return false;
}

static vector<fs::path> getUnanalyzedPaths(const string dirPath, const bool recursive, const Palette &palette)
{
	vector<fs::path> paths;

	if (recursive)
	{
		for (auto &entry : fs::recursive_directory_iterator(dirPath))
			if (isImgFile(entry.path()) && !paletteContains(palette, entry.path().string()))
				paths.push_back(entry.path());
	}
	else
	{
		for (auto &entry : fs::directory_iterator(dirPath))
			if (isImgFile(entry.path()) && !paletteContains(palette, entry.path().string()))
				paths.push_back(entry.path());
	}

	return paths;
}

void analyzeImgs(const Arguments &args)
{
	// 1. Initialize palette
	Palette palette = loadPalette(args.profile);

	// 2. Get paths that haven't been analyzed
	vector<fs::path> paths = getUnanalyzedPaths(args.analysis.dirPath, args.analysis.recursive, palette);
	cout << "Got " << paths.size() << " unanalyzed images." << endl;

	// 3. Iterate over each path
	int newImgSections = 0;
	int pathsAnalyzed = 0;
	for (fs::path path : paths)
	{
		vector<ImageSection> imgSections = splitImg(path);

		for (ImageSection imgSection : imgSections)
		{
			cv::Mat sectionImg = imreadImgSection(imgSection);
			cv::Vec3b averageColor = getAverageColor(sectionImg);
			paletteAddImgSection(palette, averageColor, imgSection);
			newImgSections += 1;
		}

		pathsAnalyzed += 1;
		cout << '\r' << pathsAnalyzed << "/" << paths.size() << " images analyzed.";
	}
	cout << "\nAdded " << newImgSections << " new image sections to the palette." << endl;

	// 4. Save palette
	savePalette(args.profile, palette);
	cout << "Saved palette." << endl;
}