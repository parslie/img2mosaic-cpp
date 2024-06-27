#include "progress.hpp"

#include <sstream>

Progress::Progress(unsigned int total) : m_current{ 0 }, m_total{ total }
{
}

// TODO: add ETA to resulting string
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
    ss << "] " << percentage << "% | " << m_current << "/" << m_total;
    return ss.str();
}

void Progress::increment(unsigned int value)
{
    if (m_current + value > m_total)
        m_current = m_total;
    else
        m_current += value;
}

unsigned int Progress::total() const
{
    return m_total;
}
