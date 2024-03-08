#pragma once

#include "../utils/image.hpp"
#include "../utils/color.hpp"

#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, std::vector<ImageSection>> Palette;

Palette loadPalette(const std::string profile);
void savePalette(const std::string profile, const Palette &palette);
void printPalette(const Palette &palette);

bool paletteContains(const Palette &palette, const std::string targetPath);
void paletteAddImgSection(Palette &palette, const Color &color, const ImageSection &imgSection);
Image paletteGetImg(Palette &palette, const Color &color, uint size);