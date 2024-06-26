#include "analysis.hpp"
#include "../data/palette.hpp"
#include <iostream>

using namespace std;
namespace fs = filesystem;

static bool is_img_file(const fs::path path) {
	string EXTS[] = { ".png", ".jpg", ".jpeg", ".webp" };
	for (string ext : EXTS)
		if (ext == path.extension())
			return true;
	return false;
}

static vector<fs::path> get_unanalyzed_paths(const string dir_path, const bool recursive, const Palette &palette) {
	vector<fs::path> paths;

	// TODO: better error-handling?
	if (recursive) {
		for (auto &entry : fs::recursive_directory_iterator(dir_path)) {
			try {
				if (is_img_file(entry.path()) && !palette_contains(palette, entry.path().string()))
					paths.push_back(entry.path());
			} catch (...) {
				cout << "An error occurred when getting a path." << endl;
			}
		}
	} else {
		for (auto &entry : fs::directory_iterator(dir_path)) {
			try {
				if (is_img_file(entry.path()) && !palette_contains(palette, entry.path().string()))
					paths.push_back(entry.path());
			} catch (...) {
				cout << "An error occurred when getting a path." << endl;
			}
		}
	}

	return paths;
}

void analyze_images(const Arguments &args) {
	cout << "[Loading palette...]" << endl;
	Palette palette = load_palette(args.profile);

	cout << "[Getting unanalyzed images...]" << endl;
	vector<fs::path> paths = get_unanalyzed_paths(args.analysis.dir_path, args.analysis.recursive, palette);

	cout << "[Analyzing " << paths.size() << " images...]" << endl;
	uint new_img_sections = 0;
	uint paths_analyzed = 0;
	try {
		for (fs::path path : paths) {
			Image image(path);
			vector<ImageSection> image_sections = image.split();

			for (ImageSection image_section : image_sections) {
				Image section_image = image_section.to_image(16);
				Color average_color = section_image.average_color();
				palette_add_img_section(palette, average_color, image_section);
				new_img_sections++;
			}

			paths_analyzed++;
			cout << "\r" << paths_analyzed << "/" << paths.size() << " images analyzed.";
		}
	} catch (...) {
		// TODO: better error handling
		cout << "An error occurred!" << endl;
	}
	cout << "\nAdded " << new_img_sections << " image sections to the palette!" << endl;

	cout << "[Saving palette...]" << endl;
	save_palette(args.profile, palette);
	cout << "Saved palette!" << endl; // TODO: maybe add whereto
}
