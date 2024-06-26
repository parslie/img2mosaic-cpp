#include "argument.hpp"
#include "commands/generation.hpp"
#include "commands/analysis.hpp"
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
	Arguments args = parse_argv(argc, argv);
	/*
	Arguments args;
	args.profile = "testing";
	args.analysis.parsed = true;
	args.analysis.dir_path = "E:\\_Gammalt\\Pictures\\Saved Pictures";
	*/

	if (args.generation.parsed)
		generate_image(args);
	else if (args.analysis.parsed)
		analyze_images(args);

	return EXIT_SUCCESS;
}