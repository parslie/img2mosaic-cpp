#include "commands.hpp"

#include "../data/image.hpp"

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

    std::cout << "[Initializing analysis...]" << '\n';
    const std::vector<fs::path> img_paths{ get_img_paths(args.analysis_args.dir_path, args.analysis_args.recurse) };
    std::cout << TAB << "Loaded image paths." << '\n';

    for (auto &img_path : img_paths)
    {
        Image img{ img_path };
        img.show("unscaled");
        img.scale_to_cover(256);
        img.show("scaled to cover");
        img.scale_to_fit(256);
        img.show("scaled to fit");
    }

    return EXIT_SUCCESS;
}
