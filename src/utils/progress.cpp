#include "progress.hpp"

#include <sstream>

Progress::Progress(size_t total) : m_current{ 0 }, m_total{ total }
{
}

void Progress::increment(size_t value)
{
    if (m_current + value > m_total)
        m_current = m_total;
    else
        m_current += value;
}

size_t Progress::total() const
{
    return m_total;
}

// [====    ] 50% | ETA: 15s | 15/30 
std::string Progress::to_string() const
{
    double factor{ (m_total == 0) ? 1.0 : m_current / static_cast<double>(m_total) };
    double percentage{ factor * 100.0 };

    const unsigned int total_cells{ 12 };
    unsigned int filled_cells{ static_cast<unsigned int>(floor(factor * total_cells)) };
    unsigned int empty_cells{ total_cells - filled_cells };

    std::stringstream ss{};
    ss << '[';
    for (unsigned int i{ 0 }; i < filled_cells; i++)
        ss << '=';
    for (unsigned int i{ 0 }; i < empty_cells; i++)
        ss << ' ';
    ss << "] ";
    ss << static_cast<int>(percentage) << "% | ";
    ss << m_current << "/" << m_total;
    return ss.str();
}
