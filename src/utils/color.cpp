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
