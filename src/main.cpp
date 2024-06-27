#include "commands/commands.hpp"
#include "utils/progress.hpp"
#include "arguments.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
    const Arguments args{ argc, argv };

    if (args.generation_args.parsed)
        return generate(args);
    else if (args.analysis_args.parsed)
        return analyze(args);
    else
        return EXIT_FAILURE;
}