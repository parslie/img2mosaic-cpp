#include "arguments.hpp"

#include "CLI/CLI.hpp"

#include <sstream>

Arguments::Arguments(int argc, char **argv)
{
    CLI::App app("Creates mosaics out of other images", "img2mosaic");
    app.add_option("profile", profile, "The collection of images to use");
    app.add_option("-d,--density", density, "The amount of pixels an image should represent (2 -> 2x2 pixels)");
    app.add_option("-c,--color", color, "The amount to divide the color space with");

    CLI::App *generation_app = app.add_subcommand("generate", "Generates a mosaic using the profiles images");
    generation_app->add_option("src", generation_args.src_path, "The image to create a mosaic of");
    generation_app->add_option("dst", generation_args.dst_path, "The path to output the mosaic to");
    generation_app->add_option("-s,--src-size", generation_args.src_size, ""); // TODO: add desc
    generation_app->add_option("-p,--pixel-size", generation_args.pixel_size, ""); // TODO: add desc

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
    constexpr std::string_view tab{ "   " };
    std::stringstream ss;

    ss << "Arguments {\n";
    ss << tab << "profile = \"" << profile << "\"\n";
    ss << tab << "density = " << density << '\n';
    ss << tab << "color = " << color << '\n';

    ss << tab << "generation_args = {\n";
    ss << tab << tab << "parsed = " << generation_args.parsed << '\n';
    ss << tab << tab << "src_path = " << generation_args.src_path << '\n';
    ss << tab << tab << "dst_path = " << generation_args.dst_path << '\n';
    ss << tab << tab << "src_size = " << generation_args.src_size << '\n';
    ss << tab << tab << "pixel_size = " << generation_args.pixel_size << '\n';
    ss << tab << "}\n";

    ss << tab << "analysis_args = {\n";
    ss << tab << tab << "parsed = " << analysis_args.parsed << '\n';
    ss << tab << tab << "src_path = " << analysis_args.dir_path << '\n';
    ss << tab << tab << "recurse = " << analysis_args.recurse << '\n';
    ss << tab << "}\n";
    ss << "}\n";

    return ss.str();
}
