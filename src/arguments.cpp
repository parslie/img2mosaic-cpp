#include "arguments.hpp"

#include "CLI/CLI.hpp"

#include <sstream>

Arguments::Arguments(int argc, char **argv)
{
    CLI::App app("Creates mosaics out of other images", "img2mosaic");
    app.add_option("profile", profile, "The collection of images to use");
    app.add_option("-d,--density", density, "The amount of pixels an image should represent (2 -> 2x2 pixels) (NOT USED YET)");
    app.add_option("-c,--color", color, "The amount to divide the color space with (NOT USED YET)");

    CLI::App *generation_app = app.add_subcommand("generate", "Generates a mosaic using the profiles images");
    generation_app->add_option("src", generation_args.src_path, "The image to create a mosaic of");
    generation_app->add_option("dst", generation_args.dst_path, "The path to output the mosaic to");
    generation_app->add_option("-s,--src-size", generation_args.src_size, "Size of the image to create a mosaic of");
    generation_app->add_option("-p,--pixel-size", generation_args.pixel_size, "Size of the images to use as pixels");

    CLI::App *analysis_app = app.add_subcommand("analyze", "Analyzes images to include in the profiles");
    analysis_app->add_option("dir", analysis_args.dir_path, "The directory of images");
    analysis_app->add_flag("-r,--recurse", analysis_args.recurse, "Recurse into subdirectories");

    try
    {
        app.parse(argc, argv);
        generation_args.parsed = generation_app->parsed();
        analysis_args.parsed = analysis_app->parsed();
    }
    catch (const CLI::ParseError &e)
    {
        const int exit_code = app.exit(e);
        exit(exit_code);
    }
}

std::string Arguments::to_string() const
{
    constexpr std::string_view TAB{ "   " };
    std::stringstream ss;

    ss << "Arguments {\n";
    ss << TAB << "profile = \"" << profile << "\"\n";
    ss << TAB << "density = " << density << '\n';
    ss << TAB << "color = " << color << '\n';

    ss << TAB << "generation_args = {\n";
    ss << TAB << TAB << "parsed = " << generation_args.parsed << '\n';
    ss << TAB << TAB << "src_path = " << generation_args.src_path << '\n';
    ss << TAB << TAB << "dst_path = " << generation_args.dst_path << '\n';
    ss << TAB << TAB << "src_size = " << generation_args.src_size << '\n';
    ss << TAB << TAB << "pixel_size = " << generation_args.pixel_size << '\n';
    ss << TAB << "}\n";

    ss << TAB << "analysis_args = {\n";
    ss << TAB << TAB << "parsed = " << analysis_args.parsed << '\n';
    ss << TAB << TAB << "src_path = " << analysis_args.dir_path << '\n';
    ss << TAB << TAB << "recurse = " << analysis_args.recurse << '\n';
    ss << TAB << "}\n";
    ss << "}\n";

    return ss.str();
}
