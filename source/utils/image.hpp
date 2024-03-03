#pragma once

#include <filesystem>
#include <vector>

#include <opencv2/opencv.hpp>

struct ImageSection
{
	cv::Mat mat;
	std::filesystem::path path;
	int x, y;
	int width, height;
};

std::vector<ImageSection> splitImage(const std::filesystem::path &path);
cv::Vec3b getAverageColor(const cv::Mat &mat);