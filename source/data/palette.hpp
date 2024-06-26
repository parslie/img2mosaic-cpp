#pragma once

#include "../utils/image.hpp"
#include "../utils/color.hpp"

#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, std::vector<ImageSection>> Palette;

Palette load_palette(const std::string profile);
void save_palette(const std::string profile, const Palette &palette);
void print_palette(const Palette &palette);

bool palette_contains(const Palette &palette, const std::string target_path);
void palette_add_img_section(Palette &palette, const Color &color, const ImageSection &img_section);
Image palette_get_img(Palette &palette, const Color &color, unsigned int size);
