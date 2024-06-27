#pragma once

#include <string>

class Progress
{
    unsigned int m_current;
    unsigned int m_total;
public:
    Progress(unsigned int total);
    std::string to_string() const;
    void increment(unsigned int value = 1);
    unsigned int total() const;
};
