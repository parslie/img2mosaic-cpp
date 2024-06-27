#include "utils/progress.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
    Progress progress{ 100 };
    for (unsigned int i{ 0 }; i < progress.total(); i++)
    {
        std::cout << progress.to_string() << '\n';
        progress.increment();
    }
    std::cout << progress.to_string() << '\n';
    return EXIT_SUCCESS;
}