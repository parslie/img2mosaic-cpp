#pragma once

#include <string>

#include <opencv2/opencv.hpp>

std::string colorToString(const cv::Vec3b &color);
cv::Vec3b stringToColor(const std::string &colorString);