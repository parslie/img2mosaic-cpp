#include "commands/commands.hpp"
#include "utils/progress.hpp"
#include "arguments.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
    const char *debug_argv[4]{ argv[0], "saved_pics", "analyze", "E:\\_Gammalt\\Pictures\\Saved Pictures" };
    char **debug_argv2 = const_cast<char **>(debug_argv);
    const Arguments args{ 4, debug_argv2 };

    if (args.generation_args.parsed)
        return generate(args);
    else if (args.analysis_args.parsed)
        return analyze(args);
    else
        return EXIT_FAILURE;
}