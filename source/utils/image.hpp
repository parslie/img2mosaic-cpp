#pragma once

#include "color.hpp"
#include "opencv2/opencv.hpp"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <vector>

class Image;

struct ImageSection {
	nlohmann::json to_json() const;
	Image to_image(uint size);

	std::filesystem::path path;
	uint x, y;
	uint width, height;
};

class Image {
public:
	Image(uint width, uint height, std::filesystem::path path);
	Image(std::filesystem::path path);
	void resize(uint new_width, uint new_height);
	void scale(double factor);
	double contain_in_size(uint size);
	double fit_to_size(uint size);
	Color &at(uint x, uint y);
	Color average_color();
	std::vector<ImageSection> split();
	uint get_width() const;
	uint get_height() const;
	void save();
private:
	std::vector<ImageSection> split_horizontal() const;
	std::vector<ImageSection> split_vertical() const;
	std::filesystem::path path;
	cv::Mat mat;
	uint width, height;
private:
};
