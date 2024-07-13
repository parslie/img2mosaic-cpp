#pragma once

#include <filesystem>
#include <map>

#include "nlohmann/json.hpp"
#include "opencv2/opencv.hpp"

typedef cv::Vec3b ColorBGR;

class Image;
class ImageLoader;

struct ImageSection
{
    unsigned int id;
    std::filesystem::path img_path;
    unsigned int x, y;
    unsigned int width, height;

    Image to_image(std::shared_ptr<ImageLoader> img_loader, unsigned int size = 0) const;
    nlohmann::json to_json() const;
    bool equals_loose(const ImageSection &other) const;
    bool equals_exact(const ImageSection &other) const;
};

class Image
{
    cv::Mat m_mat;
    unsigned int m_width, m_height;
    std::filesystem::path m_path;
public:
    Image(const std::filesystem::path &path);
    Image(unsigned int width, unsigned int height, const std::filesystem::path &path);

    void resize(unsigned int new_width, unsigned int new_height);
    void scale(double factor, unsigned int divisor = 0);
    void scale_to_cover(unsigned int size, unsigned int divisor = 0);
    void scale_to_fit(unsigned int size, unsigned int divisor = 0);
    ColorBGR &at(unsigned int x, unsigned int y);

    std::vector<ImageSection> split() const;
    ColorBGR average_color() const;
    unsigned int width() const;
    unsigned int height() const;
    void show(const std::string &window_name) const;
    void save() const;
};

// TODO: image loader may need to be re-thought
struct ImageLoaderItem
{
    Image img{ 1, 1, "." };
    size_t times_accessed{ 0 };
};

class ImageLoader
{
    unsigned int m_cache_misses{ 0 };
    unsigned int m_cache_hits{ 0 };
    std::map<std::filesystem::path, ImageLoaderItem> m_cache{};
public:
    Image load(const std::filesystem::path &path);

    std::string to_string() const;
};
