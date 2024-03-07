#include "palette.hpp"
#include "../utils/color.hpp"

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

void paletteAddImageSection(Palette &palette, const cv::Vec3b &color, const ImageSection &imageSection)
{
	string colorString = colorToString(color);

	vector<ImageSection> imageSections;
	if (palette.find(colorString) != palette.end())
		imageSections = palette.at(colorString);

	imageSections.push_back(imageSection);
	palette[colorString] = imageSections;
}

static cv::Vec3b getNearestColor(Palette &palette, const cv::Vec3b &targetColor)
{
	cv::Vec3b nearestColor;
	double nearestDistance = 442.0; // Just above max possible distance

	for (auto &[key, _] : palette)
	{
		cv::Vec3b color = stringToColor(key);
		double distance = getColorDistance(color, targetColor);

		if (distance < nearestDistance)
		{
			nearestColor = color;
			nearestDistance = distance;
		}
	}

	return nearestColor;
}

cv::Mat paletteGetImage(Palette &palette, const cv::Vec3b &color, unsigned int size)
{
	string colorString = colorToString(color);
	vector<ImageSection> imageSections;
	
	if (palette.contains(colorString))
	{
		imageSections = palette[colorString];
	}
	else
	{
		string nearColorString = colorToString(getNearestColor(palette, color));
		imageSections = palette[nearColorString];
	}

	int index = (int)round((double)rand() / RAND_MAX * (imageSections.size() - 1));
	ImageSection imageSection = imageSections[index];
	cv::Mat image;
	cv::resize(imreadImageSection(imageSection), image, cv::Size((int)size, (int)size));
	return image;
}
