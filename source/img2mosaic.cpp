#include "argument.hpp"
#include "commands/generation.hpp"
#include "commands/analysis.hpp"

int main(int argc, char **argv)
{
	Arguments arguments = parseArguments(argc, argv);

	if (arguments.generationArgs.parsed)
		generation::run(arguments);
	else if (arguments.analysisArgs.parsed)
		analysis::run(arguments);

	return 0;
}
