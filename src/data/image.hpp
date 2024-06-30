#pragma once

#include <filesystem>

#include "opencv2/opencv.hpp"
#include "nlohmann/json.hpp"

typedef cv::Vec3b Color;

class Image;

struct ImageSection
{
    std::filesystem::path path;
    unsigned int x, y;
    unsigned int width, height;

    Image to_image(unsigned int size = 0) const;
    nlohmann::json to_json() const;
};

class Image
{
    cv::Mat m_mat;
    std::filesystem::path m_path;
    unsigned int m_width, m_height;

    std::vector<ImageSection> split_horizontal() const;
    std::vector<ImageSection> split_vertical() const;
public:
    Image(std::filesystem::path path);
    Image(unsigned int width, unsigned int height, std::filesystem::path path);
    void resize(unsigned int new_width, unsigned int new_height);
    void scale(double factor);
    double contain_in_size(unsigned int size);
    double fit_to_size(unsigned int size);
    Color &at(unsigned int x, unsigned int y);

    unsigned int width() const;
    unsigned int height() const;
    void show(cv::String win_name) const;
    std::vector<ImageSection> split() const;
};
