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
    std::shared_ptr<ImageLoader> m_img_loader{};

    ColorBGR get_nearest_color(const ColorBGR &target_color) const;
public:
    Palette(std::string_view profile, std::shared_ptr<ImageLoader> img_loader);

    void insert(const ColorBGR &color, const ImageSection &img_section);
    Image at(const ColorBGR &color, unsigned int size = 0);

    bool contains_exact(const ImageSection &img_section) const;
    void save() const;
};
