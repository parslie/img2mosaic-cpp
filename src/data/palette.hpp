#pragma once

#include "image.hpp"

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

class Palette
{
    std::filesystem::path m_path{};
    unsigned int m_density{};
    unsigned int m_color{};
    std::map<std::string, std::vector<ImageSection>> m_image_sections{};
public:
    Palette(std::string profile, unsigned int density, unsigned int color);
    void save();
    //double color_coverage() const;
    std::filesystem::path path() const;
    std::string to_string() const;
    nlohmann::json to_json() const;
};
