#pragma once

#include <string>

class Progress
{
    size_t m_current;
    size_t m_total;
public:
    Progress(size_t total);
    std::string to_string() const;
    void increment(size_t value = 1);
    size_t total() const;
};
