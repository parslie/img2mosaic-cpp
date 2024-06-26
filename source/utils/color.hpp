#pragma once

#include "opencv2/opencv.hpp"
#include <string>

typedef cv::Vec3b Color;

std::string color_to_string(const Color &color);
Color string_to_color(const std::string &color_string);
double get_color_dist(const Color &color_a, const Color &color_b);
