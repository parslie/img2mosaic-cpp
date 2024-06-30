#include "commands.hpp"

#include "../data/image.hpp"
#include "../data/palette.hpp"
#include "../utils/progress.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

static bool is_img_file(const fs::path path)
{
    std::string EXTS[] = { ".png", ".jpg", ".jpeg", ".webp" };
    for (std::string ext : EXTS)
        if (ext == path.extension())
            return true;
    return false;
}

static bool has_legal_file_name(const fs::path path)
{
    // TODO: there's surely a better way to do this pls
    try
    {
        path.string();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

static std::vector<fs::path> get_paths(const fs::path dir_path, const bool recurse)
{
    std::vector<fs::path> paths{};
    if (recurse)
    {
        for (auto &entry : fs::recursive_directory_iterator(dir_path))
        {
            fs::path path{ entry.path() };
            if (is_img_file(path) && has_legal_file_name(path))
                paths.push_back(entry.path());
        }
    }
    else
    {
        for (auto &entry : fs::directory_iterator(dir_path))
        {
            fs::path path{ entry.path() };
            if (is_img_file(path) && has_legal_file_name(path))
                paths.push_back(entry.path());
        }
    }
    return paths;
}

static void analyze_path(Palette &palette, fs::path path, unsigned int density, unsigned int color)
{
    Image image{ path };

    std::vector<ImageSection> sections{ image.split() };
    for (ImageSection section : sections)
    {
        Image section_image{ section.to_image(512) };
        section_image.show("section image");
    }
}

int analyze(const Arguments &args)
{
    constexpr const char *TAB{ "   " };

    std::cout << "[Initalizing analysis...]" << '\n';
    Palette palette{ args.profile, args.density, args.color };

    std::cout << "[Getting files in directory...]" << '\n';
    std::vector<fs::path> paths{ get_paths(args.analysis_args.dir_path, args.analysis_args.recurse) };
    std::cout << TAB << "Got " << paths.size() << " files." << '\n';

    std::cout << "[Analyzing files...]" << '\n';
    Progress progress{ paths.size() };
    for (fs::path &path : paths)
    {
        std::cout << TAB << progress.to_string() << '\r';
        analyze_path(palette, path, args.density, args.color);
        progress.increment();
    }
    std::cout << TAB << progress.to_string() << '\n';

    std::cout << "[Saving palette...]" << '\n';
    palette.save();
    std::cout << TAB << "Palette saved to \"" << palette.path().string() << '"' << '\n';
    return EXIT_SUCCESS;
}
