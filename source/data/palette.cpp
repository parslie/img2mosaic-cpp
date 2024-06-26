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

constexpr auto TAB = "   ";

static fs::path get_palette_path(const string profile) {
	PWSTR local_path = NULL;
	HRESULT hres = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &local_path);

	fs::path palette_path = local_path;
	palette_path /= "Parslie";
	palette_path /= "img2mosaic (cpp)";
	palette_path /= profile;
	palette_path /= "palette.json";
	return palette_path;
}

Palette load_palette(const string profile) {
	fs::path palette_path = get_palette_path(profile);
	fs::create_directories(palette_path.parent_path());
	Palette palette;

	if (fs::exists(palette_path)) {
		ifstream file(palette_path);
		json data = json::parse(file); // TODO: handle corruption

		for (auto iter = data.begin(); iter != data.end(); iter++) {
			string color_string = iter.key();
			json sections_json = iter.value();
			vector<ImageSection> img_sections;

			for (auto section_iter = sections_json.begin(); section_iter != sections_json.end(); section_iter++) {
				json section_json = *section_iter;
				img_sections.push_back({
					section_json["path"],
					section_json["x"],
					section_json["y"],
					section_json["width"],
					section_json["height"],
				});
			}

			palette[color_string] = img_sections;
		}
	}

	return palette;
}

void save_palette(const string profile, const Palette &palette) {
	fs::path palette_path = get_palette_path(profile);

	json data = json::object();
	for (auto &[color_string, img_sections] : palette) {
		json imgs_json = json::array();
		for (auto &img_section : img_sections)
			imgs_json.push_back(img_section.to_json());
		data[color_string] = imgs_json;
	}

	ofstream file(palette_path);
	file << data << endl;
}

void print_palette(const Palette &palette) {
	cout << "{\n";
	for (auto iter = palette.begin(); iter != palette.end(); iter++) {
		string color_string = (*iter).first;
		vector<ImageSection> img_sections = (*iter).second;

		cout << TAB << "\"" << color_string << "\": [";
		for (auto sections_iter = img_sections.begin(); sections_iter != img_sections.end(); sections_iter++)
		{
			ImageSection img_section = *sections_iter;

			cout << "\"" << img_section.path.string() << "\""; // TODO: print whole image section?
			if (sections_iter + 1 != img_sections.end())
				cout << ", ";
		}
		cout << TAB << "]\n,";
	}
	cout << "}" << endl;
}

bool palette_contains(const Palette &palette, const string target_path) {
	for (auto &[_, img_sections] : palette)
		for (auto &img_section : img_sections)
			if (img_section.path == target_path)
				return true;
	return false;
}

void palette_add_img_section(Palette &palette, const Color &color, const ImageSection &img_section) {
	string color_string = color_to_string(color);

	vector<ImageSection> img_sections;
	if (palette.find(color_string) != palette.end())
		img_sections = palette.at(color_string);

	img_sections.push_back(img_section);
	palette[color_string] = img_sections;
}

static Color get_nearest_color(Palette &palette, const Color &target_color)
{
	Color nearest_color;
	double nearest_dist = 442.0; // Just above max possible distance

	for (auto &[key, _] : palette) {
		Color color = string_to_color(key);
		double dist = get_color_dist(color, target_color);

		if (dist < nearest_dist) {
			nearest_color = color;
			nearest_dist = dist;
		}
	}

	return nearest_color;
}

Image palette_get_img(Palette &palette, const Color &color, unsigned int size) {
	string color_string = color_to_string(color);
	vector<ImageSection> img_sections;

	if (palette.contains(color_string)) {
		img_sections = palette[color_string];
	} else {
		string nearColorString = color_to_string(get_nearest_color(palette, color));
		img_sections = palette[nearColorString];
	}

	int index = (int)round((double)rand() / RAND_MAX * (img_sections.size() - 1));
	ImageSection imgSection = img_sections[index];
	return imgSection.to_image(size);
}
