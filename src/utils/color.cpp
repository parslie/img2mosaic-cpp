#include "color.hpp"

#include <sstream>

std::string colors_to_string(const std::vector<Color> &colors)
{
    size_t separator_count = colors.size() - 1;
    std::stringstream ss{};

    for (Color color : colors)
    {
        int red = static_cast<int>(color[2]);
        int green = static_cast<int>(color[1]);
        int blue = static_cast<int>(color[0]);
        ss << red << ',' << green << ',' << blue;
        if (separator_count > 0)
        {
            ss << '|';
            --separator_count;
        }
    }

    return ss.str();
}
