#pragma once

#include <filesystem>

#include "nlohmann/json.hpp"

struct ImageSection
{
    std::filesystem::path path;
    unsigned int x, y;
    unsigned int width, height;

    nlohmann::json to_json() const;
};
