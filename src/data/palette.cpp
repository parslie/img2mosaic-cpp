#include "palette.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <Windows.h>
#include <shlobj_core.h>
#include <KnownFolders.h>

namespace fs = std::filesystem;
using json = nlohmann::json;

Palette::Palette(std::string profile, unsigned int density, unsigned int color) : m_density{ density }, m_color{ color }
{
    // TODO: implement linux variant
    PWSTR local_path = NULL;
    HRESULT hres = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &local_path);
    // TODO: handle HRESULT (https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath)

    m_path = local_path;
    m_path /= "Parslie";
    m_path /= "img2mosaic (cpp)";
    m_path /= profile;
    m_path /= std::format("density_{}", std::to_string(density));
    m_path /= std::format("color_{}", std::to_string(color));
    m_path /= "palette.json";

    if (fs::exists(m_path))
    {
        std::ifstream file{ m_path };
        json sections_object{ json::parse(file) };
        for (auto &[color_str, sections_array] : sections_object.items())
        {
            std::vector<ImageSection> sections{};
            for (auto section_object : sections_array)
            {
                sections.push_back({
                    section_object["path"],
                    section_object["x"],
                    section_object["y"],
                    section_object["width"],
                    section_object["height"],
                });
            }
            m_image_sections[color_str] = sections;
        }
    }
    else
    {
        fs::create_directories(m_path.parent_path());
    }
}

void Palette::save()
{
    json palette_json{ this->to_json() };
    std::ofstream file{ m_path };
    file << palette_json.dump(3) << std::endl;
}

double Palette::color_coverage() const
{
    unsigned int max_possible_color_count{ 256 * 256 * 256 / m_color };
    return static_cast<double>(m_image_sections.size()) / max_possible_color_count;
}

std::string Palette::to_string() const
{
    unsigned int section_count{ 0 };
    unsigned int color_count{ static_cast<unsigned int>(m_image_sections.size())};
    double color_coverage{ this->color_coverage() };

    for (auto &[color_str, sections] : m_image_sections)
        section_count += static_cast<unsigned int>(sections.size());

    std::stringstream ss{};
    ss << "Palette( colors = " << color_count << ", image_sections = " << section_count << ", color_coverage = " << color_coverage << "% )";
    return ss.str();
}

json Palette::to_json() const
{
    json sections_object{ json::object() };
    for (auto &[color_str, sections] : m_image_sections)
    {
        json sections_array{ json::array() };
        for (auto &section : sections)
            sections_array.push_back(section.to_json());
        sections_object[color_str] = sections_array;
    }
    return sections_object;
}
