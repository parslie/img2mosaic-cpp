#include "image.hpp"

// Constructors

Image::Image(const std::filesystem::path &path) : m_path{ path }
{
    m_mat = cv::imread(path.string(), cv::IMREAD_COLOR);
    m_width = static_cast<unsigned int>(m_mat.cols);
    m_height = static_cast<unsigned int>(m_mat.rows);
}

// Public non-const functions

void Image::resize(unsigned int new_width, unsigned int new_height)
{
    cv::Mat new_mat{};
    cv::Size new_size{ static_cast<int>(new_width), static_cast<int>(new_height) };
    cv::resize(m_mat, new_mat, new_size);
    m_mat = new_mat;
    m_width = new_width;
    m_height = new_height;
}

void Image::scale(double factor, unsigned int divisor)
{
    unsigned int new_width{ static_cast<unsigned int>(factor * m_width) };
    unsigned int new_height{ static_cast<unsigned int>(factor * m_height) };

    if (divisor != 0)
    {
        new_width += divisor - (new_width % divisor);
        new_height += divisor - (new_height % divisor);
    }

    this->resize(new_width, new_height);
}

void Image::scale_to_cover(unsigned int size, unsigned int divisor)
{
    double factor{ std::max(
        static_cast<double>(size) / m_width,
        static_cast<double>(size) / m_height
    ) };
    this->scale(factor, divisor);
}

void Image::scale_to_fit(unsigned int size, unsigned int divisor)
{
    double factor{ std::min(
        static_cast<double>(size) / m_width,
        static_cast<double>(size) / m_height
    ) };
    this->scale(factor, divisor);
}

// Public const functions

void Image::show(const std::string &window_name) const
{
    cv::imshow(window_name, m_mat);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
