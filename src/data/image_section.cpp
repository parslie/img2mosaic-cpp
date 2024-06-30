#include "image.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

Image ImageSection::to_image(unsigned int size) const
{
	Image full_image{ path };

	double scale_factor{ 1.0 };
	if (size == 0.0)
		size = std::min(full_image.width(), full_image.height());
	else
		scale_factor = full_image.fit_to_size(size);

	fs::path section_path{ path.stem() };
	section_path += "_section";
	section_path += this->path.extension();
	Image section_image{ size, size, section_path };

	unsigned int x_min{ static_cast<unsigned int>(floor(x * scale_factor)) };
	unsigned int x_max{ static_cast<unsigned int>(floor((x + width) * scale_factor)) };
	unsigned int y_min{ static_cast<unsigned int>(floor(y * scale_factor)) };
	unsigned int y_max{ static_cast<unsigned int>(floor((y + height) * scale_factor)) };

	for (unsigned int y_curr{ y_min }; y_curr < y_max; y_curr++)
	{
		for (unsigned int x_curr{ x_min }; x_curr < x_max; x_curr++)
		{
			unsigned int section_x{ x_curr - x_min };
			unsigned int section_y{ y_curr - y_min };
			section_image.at(section_x, section_y) = full_image.at(x_curr, y_curr);
		}
	}

	return section_image;
}

json ImageSection::to_json() const
{
	json data{ json::object() };
	data["path"] = path.string();
	data["x"] = (int)(x);
	data["y"] = (int)(y);
	data["width"] = (int)(width);
	data["height"] = (int)(height);
	return data;
}
