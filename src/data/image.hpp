#pragma once

#include <filesystem>

#include "opencv2/opencv.hpp"

typedef cv::Vec3b ColorBGR;

class Image
{
    cv::Mat m_mat;
    unsigned int m_width, m_height;
    std::filesystem::path m_path;
public:
    Image(const std::filesystem::path &path);

    void resize(unsigned int new_width, unsigned int new_height);
    void scale(double factor, unsigned int divisor = 0);
    void scale_to_cover(unsigned int size, unsigned int divisor = 0);
    void scale_to_fit(unsigned int size, unsigned int divisor = 0);
    ColorBGR &at(unsigned int x, unsigned int y);

    unsigned int width() const;
    unsigned int height() const;
    void show(const std::string &window_name) const;
    void save() const;
};
