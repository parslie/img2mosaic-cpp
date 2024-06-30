#include "image.hpp"

Image::Image(std::filesystem::path path) : m_path{ path }
{
    m_mat = cv::imread(path.string(), cv::IMREAD_COLOR);
    m_width = m_mat.cols;
    m_height = m_mat.rows;
}

Image::Image(unsigned int width, unsigned int height, std::filesystem::path path)
    : m_path{ path }, m_width{ width }, m_height{ height }
{
    m_mat = cv::Mat::zeros(static_cast<int>(height), static_cast<int>(width), CV_8UC3);
}

void Image::resize(unsigned int new_width, unsigned int new_height)
{
    cv::Mat resized_mat{};
    cv::Size new_size{ static_cast<int>(new_width), static_cast<int>(new_height) };
    cv::resize(m_mat, resized_mat, new_size);
    m_mat = resized_mat;
    m_width = new_width;
    m_height = new_height;
}

void Image::scale(double factor)
{
    unsigned int new_width{ static_cast<unsigned int>(ceil(factor * m_width)) };
    unsigned int new_height{ static_cast<unsigned int>(ceil(factor * m_height)) };
    this->resize(new_width, new_height);
}

double Image::contain_in_size(unsigned int size)
{
    double scale_factor = std::min(
        (double)size / m_width,
        (double)size / m_height
    );

    if (scale_factor < 1.0) {
        this->scale(scale_factor);
        return scale_factor;
    }

    return 1.0;
}

double Image::fit_to_size(unsigned int size)
{
    double scale_factor = std::max(
        (double)size / m_width,
        (double)size / m_height
    );

    if (scale_factor < 1.0) {
        this->scale(scale_factor);
        return scale_factor;
    }

    return 1.0;
}

Color &Image::at(unsigned int x, unsigned int y)
{
    return m_mat.at<Color>((int)y, (int)x);
}

unsigned int Image::width() const
{
    return m_width;
}

unsigned int Image::height() const
{
    return m_height;
}

void Image::show(cv::String win_name) const
{
    cv::imshow(win_name, m_mat);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

std::vector<ImageSection> Image::split() const
{
    if (m_width > m_height)
        return this->split_horizontal();
    else
        return this->split_vertical();
}

std::vector<ImageSection> Image::split_horizontal() const
{
    unsigned int section_size = std::min(m_width, m_height);
    double cols = static_cast<double>(m_width) / static_cast<double>(section_size);

    std::vector<ImageSection> sections{};
    for (unsigned int col{ 0 }; col < cols; col++) {
        unsigned int x_min{ section_size * col };
        unsigned int x_max{ section_size * (col + 1) };
        unsigned int y_min{ 0 };
        unsigned int y_max{ section_size };

        if (x_max > m_width) {
            unsigned int diff{ x_max - m_width };
            x_max -= diff;
            x_min -= diff;
        }

        ImageSection section = {
            m_path,
            x_min,
            y_min,
            section_size,
            section_size
        };
        sections.push_back(section);
    }

    return sections;
}

std::vector<ImageSection> Image::split_vertical() const
{
    unsigned int section_size = std::min(m_width, m_height);
    double rows = static_cast<double>(m_height) / static_cast<double>(section_size);

    std::vector<ImageSection> sections{};
    for (unsigned int row{ 0 }; row < rows; row++) {
        unsigned int x_min{ 0 };
        unsigned int x_max{ section_size };
        unsigned int y_min{ section_size * row };
        unsigned int y_max{ section_size * (row + 1) };

        if (y_max > m_height) {
            uint diff{ y_max - m_height };
            y_max -= diff;
            y_min -= diff;
        }

        ImageSection section = {
            m_path,
            x_min,
            y_min,
            section_size,
            section_size
        };
        sections.push_back(section);
    }

    return sections;
}
