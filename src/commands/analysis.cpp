#include "commands.hpp"

#include "../data/palette.hpp"

#include <cstdlib>
#include <iostream>

int analyze(const Arguments &args)
{
    std::cout << "[Initalizing analysis...]" << '\n';
    Palette palette{ args.profile, args.density, args.color };
    std::cout << palette.to_string() << std::endl;

    std::cout << "[Saving analyzed images to palette...]" << '\n';
    palette.save();
    return EXIT_SUCCESS;
}
