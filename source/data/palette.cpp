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
		string color = iter.key();
		json imagesJson = iter.value();
		vector<string> images;

		for (auto imagesIter = imagesJson.begin(); imagesIter != imagesJson.end(); imagesIter++)
			images.push_back(*imagesIter);

		palette[color] = images;
	}

	return palette;
}

void savePalette(string profile, const Palette &palette)
{
	fs::path palettePath = getPalettePath(profile);

	json data = json::object();
	for (auto &[color, images] : palette)
	{
		json imagesJson = json::array();
		for (auto image : images)
			imagesJson.push_back(image);
		data[color] = imagesJson;
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
		string key = (*iter).first;
		vector<string> list = (*iter).second;

		cout << TAB << "\"" << key << "\": [";
		for (auto listIter = list.begin(); listIter != list.end(); listIter++)
		{
			string listValue = *listIter;

			cout << "\"" << listValue << "\"";
			if (listIter + 1 != list.end())
				cout << ", ";
		}
		cout << TAB << "]\n,";
	}
	cout << "}" << endl;
}

bool paletteContains(const Palette &palette, const string targetPath)
{
	for (auto &[_, paths] : palette)
		for (auto &path : paths)
			if (path == targetPath)
				return true;

	return false;
}
