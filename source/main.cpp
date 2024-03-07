#include "argument.hpp"
#include "commands/generation.hpp"
#include "commands/analysis.hpp"

int main(int argc, char **argv)
{
	Arguments args = parseArgs(argc, argv);

	if (args.generation.parsed)
		generateImg(args);
	else if (args.analysis.parsed)
		analyzeImgs(args);

	return 0;
}
