#include "image.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

json ImageSection::to_json() const {
	json data = json::object();
	data["path"] = this->path.string();
	data["x"] = (int)(this->x);
	data["y"] = (int)(this->y);
	data["width"] = (int)(this->width);
	data["height"] = (int)(this->height);
	return data;
}

Image ImageSection::to_image(uint size) {
	Image full_img(this->path);

	double scale_factor;
	if (size == 0.0) {
		size = min(full_img.get_width(), full_img.get_height());
		scale_factor = 1.0;
	} else {
		scale_factor = full_img.fit_to_size(size);
	}

	fs::path section_path = this->path.stem();
	section_path += "_section";
	section_path += this->path.extension();
	Image section_img(size, size, section_path);

	uint x_min = (uint)floor(this->x * scale_factor);
	uint x_max = (uint)floor((this->x + this->width) * scale_factor);
	uint y_min = (uint)floor(this->y * scale_factor);
	uint y_max = (uint)floor((this->y + this->height) * scale_factor);

	for (uint y = y_min; y < y_max; y++) {
		for (uint x = x_min; x < x_max; x++) {
			uint section_x = x - x_min;
			uint section_y = y - y_min;
			section_img.at(section_x, section_y) = full_img.at(x, y);
		}
	}

	return section_img;
}
