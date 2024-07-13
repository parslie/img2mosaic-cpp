#include "color.hpp"

constexpr char CHANNEL_DELIMITER = ',';

std::string color_to_str(const ColorBGR &color)
{
    std::stringstream ss{};
    int red{ static_cast<int>(color[2]) };
    int green{ static_cast<int>(color[1]) };
    int blue{ static_cast<int>(color[0]) };
    ss << red << CHANNEL_DELIMITER << green << CHANNEL_DELIMITER << blue;
    return ss.str();
}

ColorBGR str_to_color(std::string_view str)
{
    size_t first_delim{ str.find(CHANNEL_DELIMITER) };
    size_t second_delim{ str.find(CHANNEL_DELIMITER, first_delim) };

    std::string red_str{ str.substr(0, first_delim) };
    std::string green_str{ str.substr(first_delim + 1, second_delim) };
    std::string blue_str{ str.substr(second_delim + 1) };

    uchar red{ static_cast<uchar>(std::stoi(red_str)) };
    uchar green{ static_cast<uchar>(std::stoi(green_str)) };
    uchar blue{ static_cast<uchar>(std::stoi(blue_str)) };
    return ColorBGR(blue, green, red);
}

double calc_color_dist(const ColorBGR &a, const ColorBGR &b)
{
    double a_red = static_cast<double>(a[2]);
    double a_green = static_cast<double>(a[1]);
    double a_blue = static_cast<double>(a[0]);
    double b_red = static_cast<double>(b[2]);
    double b_green = static_cast<double>(b[1]);
    double b_blue = static_cast<double>(b[0]);

    double sqr_red_diff = pow(a_red - b_red, 2);
    double sqr_green_diff = pow(a_green - b_green, 2);
    double sqr_blue_diff = pow(a_blue - b_blue, 2);

    return sqrt(sqr_red_diff + sqr_green_diff + sqr_blue_diff);
}
