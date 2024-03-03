#pragma once

#include <filesystem>
#include <vector>

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

struct ImageSection
{
	cv::Mat mat;
	std::filesystem::path path;
	int x, y;
	int width, height;

	nlohmann::json toJson() const;
};

std::vector<ImageSection> splitImage(const std::filesystem::path &path);
cv::Vec3b getAverageColor(const cv::Mat &mat);