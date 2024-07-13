#include "palette.hpp"

#include "../utils/color.hpp"

#include <fstream>

#include <shlobj_core.h>
#include <KnownFolders.h>

#include "nlohmann/json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

static std::map<std::string, std::vector<ImageSection>> load_img_sections(const fs::path &path)
{
    // TODO: handle corruption or ill-formatted json
    std::ifstream file{ path };
    json object{ json::parse(file) };
    std::map<std::string, std::vector<ImageSection>> img_sections{};

    for (auto [color_str, sections_arr] : object.items())
    {
        std::vector<ImageSection> sections{};
        for (auto section_obj : sections_arr)
        {
            sections.push_back({
                section_obj["id"],
                section_obj["img_path"],
                section_obj["x"],
                section_obj["y"],
                section_obj["width"],
                section_obj["height"]
            });
        }
        img_sections[color_str] = sections;
    }

    return img_sections;
}

Palette::Palette(std::string_view profile, std::shared_ptr<ImageLoader> img_loader) : m_img_loader{ img_loader }
{
    PWSTR local_path{ NULL };
    HRESULT h_res{ SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &local_path)};
    // TODO: linux variant

    m_path = local_path;
    m_path /= "Parslie";
    m_path /= "img2mosaic (cpp)";
    m_path /= profile;
    m_path /= "palette.json";

    if (fs::exists(m_path))
        m_img_sections = load_img_sections(m_path);
    else
        fs::create_directories(m_path.parent_path());
}

// Public non-const functions

void Palette::insert(const ColorBGR &color, const ImageSection &img_section)
{
    std::string color_str{ color_to_str(color) };
    if (m_img_sections.contains(color_str))
    {
        std::vector<ImageSection> &img_sections{ m_img_sections.at(color_str) };
        bool exists{ false };
        size_t idx{ 0 };

        for (size_t curr_idx{ 0 }; curr_idx < img_sections.size(); ++curr_idx)
        {
            const ImageSection &other_img_section{ img_sections.at(curr_idx) };
            if (img_section.equals_loose(other_img_section))
            {
                exists = true;
                idx = curr_idx;
                break;
            }
        }

        if (exists)
            img_sections[idx] = img_section;
        else
            img_sections.push_back(img_section);
    }
    else
    {
        std::vector<ImageSection> img_sections{ img_section };
        m_img_sections[color_str] = img_sections;
    }
}

Image Palette::at(const ColorBGR &color, unsigned int size)
{
    std::string color_str{ color_to_str(color) };
    if (!m_img_sections.contains(color_str))
        color_str = color_to_str(this->get_nearest_color(color));

    std::vector<ImageSection> &img_sections{ m_img_sections.at(color_str) };

    double rand_val{ rand() / static_cast<double>(RAND_MAX) };
    size_t idx{ static_cast<size_t>(rand_val * img_sections.size()) };

    ImageSection &img_section{ img_sections.at(idx) };
    return img_section.to_image(m_img_loader, size);
}

bool Palette::contains_exact(const ImageSection &img_section) const
{
    for (auto &[_, other_img_sections] : m_img_sections)
        for (auto &other_img_section : other_img_sections)
            if (img_section.equals_exact(other_img_section))
                return true;
    return false;
}

void Palette::save() const
{
    json palette_obj{ json::object() };
    for (auto &[color_str, img_sections] : m_img_sections)
    {
        json sections_array{ json::array() };
        for (auto &img_section : img_sections)
            sections_array.push_back(img_section.to_json());
        palette_obj[color_str] = sections_array;
    }

    std::ofstream file{ m_path };
    file << palette_obj.dump(3) << std::endl;
}

// Private const functions

ColorBGR Palette::get_nearest_color(const ColorBGR &target_color) const
{
    ColorBGR nearest_color;
    double nearest_dist{ 442.0 }; // Just above max possible distance

    for (auto &[color_str, _] : m_img_sections)
    {
        ColorBGR color{ str_to_color(color_str) };
        double dist{ calc_color_dist(color, target_color) };
        if (dist < nearest_dist)
        {
            nearest_color = color;
            nearest_dist = dist;
        }
    }

    return nearest_color;
}
