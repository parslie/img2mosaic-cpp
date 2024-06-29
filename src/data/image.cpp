#include "image.hpp"

using json = nlohmann::json;

json ImageSection::to_json() const
{
	json data{ json::object() };
	data["path"] = this->path.string();
	data["x"] = (int)(this->x);
	data["y"] = (int)(this->y);
	data["width"] = (int)(this->width);
	data["height"] = (int)(this->height);
	return data;
}
