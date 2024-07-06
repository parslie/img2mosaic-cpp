#include "palette.hpp"

#include "../utils/color.hpp"

#include <fstream>

#include <shlobj_core.h>
#include <KnownFolders.h>

#include "nlohmann/json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

Palette::Palette(std::string_view profile)
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
    {
        std::cout << "TODO: implement palette loading" << '\n';
    }
    else
    {
        fs::create_directories(m_path.parent_path());
    }
}

void Palette::insert(const ColorBGR &color, const ImageSection &img_section)
{
    std::string color_str{ color_to_str(color) };
    if (m_img_sections.contains(color_str))
    {
        std::vector<ImageSection> &img_sections{ m_img_sections.at(color_str) };
        img_sections.push_back(img_section);
        // TODO: do not push if already exists
        // TODO: if images change size, an ID needs to be added to the img_section to compare them
        // TODO: and in case the image section exists (and is not fully equal in size), replace it instead
    }
    else
    {
        std::vector<ImageSection> img_sections{ img_section };
        m_img_sections[color_str] = img_sections;
    }
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
