#include "commands.hpp"

#include "../data/image.hpp"
#include "../data/palette.hpp"
#include "../utils/color.hpp"
#include "../utils/progress.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

static bool is_img_file(const fs::path &img_path)
{
    constexpr std::string_view EXTS[]{ ".png", ".jpg", ".jpeg", ".webp" };
    for (auto ext : EXTS)
        if (ext == img_path.extension())
            return true;
    return false;
}

static bool is_valid_img_entry(const fs::directory_entry &entry)
{
    try
    {
        // Fails on Windows if the file name is not valid
        std::string path{ entry.path().string() };
    }
    catch (...)
    {
        return false;
    }

    return is_img_file(entry.path());
} 

static std::vector<fs::path> get_img_paths(const fs::path &dir_path, const bool recurse)
{
    std::vector<fs::path> img_paths{};

    if (recurse)
    {
        for (auto entry : fs::recursive_directory_iterator(dir_path))
            if (is_valid_img_entry(entry))
                img_paths.push_back(entry.path());
    }
    else
    {
        for (auto entry : fs::directory_iterator(dir_path))
            if (is_valid_img_entry(entry))
                img_paths.push_back(entry.path());
    }

    return img_paths;
}

int analyze(const Arguments &args)
{
    constexpr const char *TAB{ "   " };

    std::cout << "[Preparing analysis...]" << '\n';
    std::shared_ptr<ImageLoader> img_loader{ std::make_shared<ImageLoader>() };
    Palette palette{ args.profile, img_loader };
    std::cout << TAB << "Loaded palette." << '\n';
    const std::vector<fs::path> img_paths{ get_img_paths(args.analysis_args.dir_path, args.analysis_args.recurse) };
    std::cout << TAB << "Loaded image paths." << '\n';

    std::cout << "[Starting analysis...]" << '\n';
    Progress progress{ img_paths.size() };
    for (const fs::path &img_path : img_paths)
    {
        std::cout << TAB << progress.to_string() << '\r';

        Image img{ img_path };
        for (ImageSection img_section : img.split())
        {
            if (palette.contains_exact(img_section))
                continue; // TODO: can probably break here
            Image section_img{ img_section.to_image(img_loader, 512) };
            ColorBGR average_color{ section_img.average_color() };
            palette.insert(average_color, img_section);
        }
        progress.increment();
    }
    std::cout << TAB << progress.to_string() << '\n';

    std::cout << "[Saving palette...]" << '\n';
    palette.save();
    std::cout << TAB << "Saved palette." << '\n';

    return EXIT_SUCCESS;
}
