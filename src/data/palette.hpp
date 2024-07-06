#pragma once

#include "image.hpp"

#include <filesystem>
#include <map>
#include <string>
#include <vector>

class Palette
{
    std::filesystem::path m_path{};
    std::map<std::string, std::vector<ImageSection>> m_img_sections{};
public:
    Palette(std::string_view profile);

    void insert(const ColorBGR &color, const ImageSection &img_section);

    void save() const;
};
