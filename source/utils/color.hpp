#pragma once

#include <string>

#include <opencv2/opencv.hpp>

typedef cv::Vec3b Color;

std::string colorToString(const Color &color);
Color stringToColor(const std::string &colorString);
double getColorDist(const Color &colorA, const Color &colorB);