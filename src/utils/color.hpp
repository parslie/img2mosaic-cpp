#pragma once

#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

typedef cv::Vec3b Color;

std::string colors_to_string(const std::vector<Color> &colors);
