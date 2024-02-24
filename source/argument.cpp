#include "argument.hpp"
#include "cxxopts.hpp"
#include "CLI/CLI.hpp"
#include <iostream>

Arguments parseArguments(int argc, const char* const* argv)
{
	Arguments arguments = {};

	CLI::App app("A program that generates mosaics of images made of other images.", "img2mosaic");

    try {                            
		app.parse(argc, argv);                                                                                    \
    } catch(const CLI::ParseError &e) {                                                                                \
        exit(app.exit(e));                                                                                          \
    }

	return arguments;
}