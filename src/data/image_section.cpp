#include "image.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

Image ImageSection::to_image(unsigned int size) const
{
	Image full_img{ img_path };

	fs::path section_path = img_path.stem();
	section_path += "_section";
	section_path += "_";
	section_path += std::to_string(x);
	section_path += "x";
	section_path += std::to_string(y);
	section_path += "_";
	section_path += std::to_string(width);
	section_path += "x";
	section_path += std::to_string(height);
	section_path += img_path.extension();

	Image section_img{ width, height, section_path };
	for (unsigned int section_y{ 0 }; section_y < height; ++section_y)
	{
		for (unsigned int section_x{ 0 }; section_x < width; ++section_x)
		{
			unsigned int full_x{ x + section_x };
			unsigned int full_y{ y + section_y };
			section_img.at(section_x, section_y) = full_img.at(full_x, full_y);
		}
	}

	// TODO: resize should be done before copying pixels to increase speed
	if (size != 0)
		section_img.scale_to_cover(size);
	return section_img;
}

json ImageSection::to_json() const
{
	json object{ json::object() };
	object["img_path"] = img_path;
	object["x"] = x;
	object["y"] = y;
	object["width"] = width;
	object["height"] = height;
	return object;
}
