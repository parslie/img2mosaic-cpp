#pragma once

#include <filesystem>
#include <string>

struct GenerationArguments
{
    bool parsed{ false };
    std::filesystem::path src_path{};
    std::filesystem::path dst_path{};
    unsigned int src_size{ 128 };
    unsigned int pixel_size{ 32 };
};

struct AnalysisArguments
{
    bool parsed{ false };
    std::filesystem::path dir_path{};
    bool recurse{ false };
};

struct Arguments
{
    GenerationArguments generation_args{};
    AnalysisArguments analysis_args{};
    std::string profile{ "default" };
    unsigned int density{ 1 };
    unsigned int color{ 1 };

    Arguments(int argc, char **argv);
    std::string to_string() const;
};
