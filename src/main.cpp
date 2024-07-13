#include "arguments.hpp"
#include "commands/commands.hpp"

Arguments debug_analysis_args(int argc, char **argv)
{
    const int debug_argc = 4;
    const char *const_debug_argv[debug_argc]{ argv[0], "saved_pics", "analyze", "E:\\Pictures\\Saved Pictures" };
    char **debug_argv = const_cast<char **>(const_debug_argv);
    return Arguments{ debug_argc, debug_argv };
}

Arguments debug_generation_args(int argc, char **argv)
{
    const int debug_argc = 9;
    const char *const_debug_argv[debug_argc]{ argv[0], "saved_pics", "generate", "-s", "64", "-p", "64", "E:\\Pictures\\Saved Pictures\\20220123_101647.jpg", "..\\test.jpg"};
    char **debug_argv = const_cast<char **>(const_debug_argv);
    return Arguments{ debug_argc, debug_argv };
}

int main(int argc, char **argv)
{
    //const Arguments args{ argc, argv };
    //const Arguments args{ debug_analysis_args(argc, argv) };
    const Arguments args{ debug_generation_args(argc, argv) };

    if (args.generation_args.parsed)
        return generate(args);
    else if (args.analysis_args.parsed)
        return analyze(args);
    
    return EXIT_FAILURE;
}