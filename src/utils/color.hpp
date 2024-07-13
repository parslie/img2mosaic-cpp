#pragma once

#include "../data/image.hpp"

#include <string>

std::string color_to_str(const ColorBGR &color);
ColorBGR str_to_color(std::string_view str);
double calc_color_dist(const ColorBGR &a, const ColorBGR &b);
