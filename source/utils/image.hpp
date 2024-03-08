#pragma once

#include "color.hpp"

#include <filesystem>
#include <vector>

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

class Image;

struct ImageSection
{
	nlohmann::json toJson() const;
	Image toImage(uint size);

	std::filesystem::path path;
	uint x, y;
	uint width, height;
};

class Image
{
public:
	Image(uint width, uint height, std::filesystem::path path);
	Image(std::filesystem::path path);
	void resize(uint width, uint height);
	double containInSize(uint size);
	double fitToSize(uint size);
	Color &at(uint x, uint y);
	Color averageColor();
	std::vector<ImageSection> split();
	uint getWidth() const;
	uint getHeight() const;
	void save();
private:
	std::vector<ImageSection> splitHorizontal() const;
	std::vector<ImageSection> splitVertical() const;
	std::filesystem::path path;
	cv::Mat mat;
	uint width, height;
};
