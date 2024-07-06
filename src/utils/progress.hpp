#pragma once

#include <ctime>
#include <string>

class Progress
{
    size_t m_current{};
    size_t m_total{};
public:
    Progress(size_t total);

    void increment(size_t value = 1);

    size_t total() const;
    std::string to_string() const;
};
