#include "arguments.hpp"
#include "commands/commands.hpp"

int main(int argc, char **argv)
{
    const Arguments args{ argc, argv };

    if (args.generation_args.parsed)
        return generate(args);
    else if (args.analysis_args.parsed)
        return analyze(args);
    
    return EXIT_FAILURE;
}