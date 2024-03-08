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

constexpr auto TAB = "   ";

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
		json imgSectionsJson = iter.value();
		vector<ImageSection> imgSections;

		for (auto imgSectionsIter = imgSectionsJson.begin(); imgSectionsIter != imgSectionsJson.end(); imgSectionsIter++)
		{
			json imgSectionJson = *imgSectionsIter;
			imgSections.push_back({
				imgSectionJson["path"],
				imgSectionJson["x"],
				imgSectionJson["y"],
				imgSectionJson["width"],
				imgSectionJson["height"],
			});
		}

		palette[colorString] = imgSections;
	}

	return palette;
}

void savePalette(string profile, const Palette &palette)
{
	fs::path palettePath = getPalettePath(profile);

	json data = json::object();
	for (auto &[colorString, imgSections] : palette)
	{
		json imgsJson = json::array();
		for (auto &imgSection : imgSections)
			imgsJson.push_back(imgSection.toJson());
		data[colorString] = imgsJson;
	}

	ofstream file(palettePath);
	file << data << endl;
}

void printPalette(const Palette &palette)
{
	cout << "{\n";
	for (auto iter = palette.begin(); iter != palette.end(); iter++)
	{
		string colorString = (*iter).first;
		vector<ImageSection> imgSections = (*iter).second;

		cout << TAB << "\"" << colorString << "\": [";
		for (auto imgSectionsIter = imgSections.begin(); imgSectionsIter != imgSections.end(); imgSectionsIter++)
		{
			ImageSection imgSection = *imgSectionsIter;

			cout << "\"" << imgSection.path.string() << "\""; // TODO: print whole image section?
			if (imgSectionsIter + 1 != imgSections.end())
				cout << ", ";
		}
		cout << TAB << "]\n,";
	}
	cout << "}" << endl;
}

bool paletteContains(const Palette &palette, const string targetPath)
{
	for (auto &[_, imgSections] : palette)
		for (auto &imgSection : imgSections)
			if (imgSection.path == targetPath)
				return true;

	return false;
}

void paletteAddImgSection(Palette &palette, const Color &color, const ImageSection &imgSection)
{
	string colorString = colorToString(color);

	vector<ImageSection> imgSections;
	if (palette.find(colorString) != palette.end())
		imgSections = palette.at(colorString);

	imgSections.push_back(imgSection);
	palette[colorString] = imgSections;
}

static Color getNearestColor(Palette &palette, const Color &targetColor)
{
	Color nearestColor;
	double nearestDist = 442.0; // Just above max possible distance

	for (auto &[key, _] : palette)
	{
		Color color = stringToColor(key);
		double dist = getColorDist(color, targetColor);

		if (dist < nearestDist)
		{
			nearestColor = color;
			nearestDist = dist;
		}
	}

	return nearestColor;
}

Image paletteGetImg(Palette &palette, const Color &color, uint size)
{
	string colorString = colorToString(color);
	vector<ImageSection> imgSections;
	
	if (palette.contains(colorString))
	{
		imgSections = palette[colorString];
	}
	else
	{
		string nearColorString = colorToString(getNearestColor(palette, color));
		imgSections = palette[nearColorString];
	}

	int index = (int)round((double)rand() / RAND_MAX * (imgSections.size() - 1));
	ImageSection imgSection = imgSections[index];
	return imgSection.toImage(size);
}
