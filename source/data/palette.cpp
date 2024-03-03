#include "palette.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <Windows.h>
#include <shlobj_core.h>
#include <KnownFolders.h>

#include <nlohmann/json.hpp>

using namespace std;
namespace fs = filesystem;
using json = nlohmann::json;

static fs::path getPalettePath(string profile)
{
	PWSTR localPath = NULL;
	HRESULT hres = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localPath);

	fs::path palettePath = localPath;
	palettePath /= "Parslie";
	palettePath /= "img2mosaic (cpp)";
	palettePath /= profile;
	palettePath /= "palette.json";

	return palettePath;
}

Palette loadPalette(string profile)
{
	fs::path palettePath = getPalettePath(profile);
	Palette palette;

	fs::create_directories(palettePath.parent_path());

	ifstream file(palettePath);
	json data = json::parse(file); // TODO: handle curruption

	for (auto iter = data.begin(); iter != data.end(); iter++)
	{
		string colorString = iter.key();
		json imageSectionsJson = iter.value();
		vector<ImageSection> imageSections;

		for (auto imageSectionsIter = imageSectionsJson.begin(); imageSectionsIter != imageSectionsJson.end(); imageSectionsIter++)
		{
			json imageSectionJson = *imageSectionsIter;
			imageSections.push_back({
				cv::Mat::zeros(0, 0, 0), // Pre-loading images here would waste memory
				imageSectionJson["path"],
				imageSectionJson["x"],
				imageSectionJson["y"],
				imageSectionJson["width"],
				imageSectionJson["height"],
			});
		}

		palette[colorString] = imageSections;
	}

	return palette;
}

void savePalette(string profile, const Palette &palette)
{
	fs::path palettePath = getPalettePath(profile);

	json data = json::object();
	for (auto &[colorString, imageSections] : palette)
	{
		json imagesJson = json::array();
		for (auto &imageSection : imageSections)
			imagesJson.push_back(imageSection.toJson());
		data[colorString] = imagesJson;
	}

	ofstream file(palettePath);
	file << data << endl;
}

constexpr auto TAB = "   ";

void printPalette(const Palette &palette)
{
	cout << "{\n";
	for (auto iter = palette.begin(); iter != palette.end(); iter++)
	{
		string colorString = (*iter).first;
		vector<ImageSection> imageSections = (*iter).second;

		cout << TAB << "\"" << colorString << "\": [";
		for (auto imageSectionsIter = imageSections.begin(); imageSectionsIter != imageSections.end(); imageSectionsIter++)
		{
			ImageSection imageSection = *imageSectionsIter;

			cout << "\"" << imageSection.path.string() << "\""; // TODO: print whole image section?
			if (imageSectionsIter + 1 != imageSections.end())
				cout << ", ";
		}
		cout << TAB << "]\n,";
	}
	cout << "}" << endl;
}

bool paletteContains(const Palette &palette, const string targetPath)
{
	for (auto &[_, imageSections] : palette)
		for (auto &imageSection : imageSections)
			if (imageSection.path == targetPath)
				return true;

	return false;
}
