#pragma once

#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, std::vector<std::string>> Palette;

Palette loadPalette(const std::string profile);
void savePalette(const std::string profile, const Palette &palette);