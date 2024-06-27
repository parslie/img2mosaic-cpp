#include "utils/progress.hpp"
#include "arguments.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
    const Arguments args{ argc, argv };
    std::cout << args.to_string() << '\n';
    return EXIT_SUCCESS;
}