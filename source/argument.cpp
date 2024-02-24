#include "argument.hpp"
#include "cxxopts.hpp"
#include "CLI/CLI.hpp"
#include <iostream>
#include <sstream>

// TODO: add validators
static CLI::App *createGenerationCommand(CLI::App* app, GenerationArgs &arguments)
{
	CLI::App *command = app->add_subcommand(
		"generate",
		"Generate a mosaic"
	);

	command->add_option<std::string>(
		"source",
		arguments.sourcePath,
		"Path to the image to mosaic"
	)->option_text("PATH")->required(true);

	command->add_option<std::string>(
		"destination",
		arguments.destinationPath,
		"Path to output the mosaic to"
	)->option_text("PATH")->required(true);

	command->add_option<unsigned int>(
		"-d,--source-size",
		arguments.sourceSize,
		"The size of the image to mosaic"
	)->option_text("PIXELS");

	command->add_option<unsigned int>(
		"-p,--pixel-size",
		arguments.pixelSize,
		"The size of the images to use as pixels"
	)->option_text("PIXELS");

	return command;
}

// TODO: add validators
static CLI::App *createAnalysisCommand(CLI::App* app, AnalysisArgs &arguments)
{
	CLI::App *command = app->add_subcommand(
		"analyze", 
		"Analyze images to represent pixels"
	);

	command->add_option<std::string>(
		"directory",
		arguments.directoryPath,
		"The directory containing the images to analyze"
	)->option_text("PATH")->required(true);

	command->add_flag<bool>(
		"-r,--recursive",
		arguments.recursive,
		"Analyze images in subdirectories as well"
	);

	return command;
}

// TODO: add validators
Arguments parseArguments(int argc, const char* const* argv)
{
	Arguments arguments;

	CLI::App app("A program that generates mosaics of images made of other images", "img2mosaic");

	app.add_option<std::string>(
		"profile",
		arguments.profile,
		"The profile to use"
	)->option_text("TEXT");

	app.add_flag<bool>(
		"--debug",
		arguments.debug,
		"Print debug statements"
	);

	CLI::App* generation = createGenerationCommand(&app, arguments.generationArgs);
	CLI::App* analysis = createAnalysisCommand(&app, arguments.analysisArgs);

    try {
		app.parse(argc, argv);
    } catch(const CLI::ParseError &e) {
        exit(app.exit(e));
    }

	arguments.generationArgs.parsed = generation->parsed();
	arguments.analysisArgs.parsed = analysis->parsed();

	if (arguments.debug) {
		std::cout << "arguments = " << arguments.toString() << std::endl;
	}

	return arguments;
}

std::string Arguments::toString()
{
	std::stringstream stringStream;
	stringStream << "{\n";
	stringStream << "\tprofile = " << this->profile << "\n";
	stringStream << "\tdebug = " << this->debug << "\n";
	stringStream << "\tgeneration = {\n";
	stringStream << "\t\tparsed = " << this->generationArgs.parsed << "\n";
	stringStream << "\t\tsource = '" << this->generationArgs.sourcePath << "'\n";
	stringStream << "\t\tdestination = '" << this->generationArgs.destinationPath << "'\n";
	stringStream << "\t\tsource size = " << this->generationArgs.sourceSize << "\n";
	stringStream << "\t\tpixel size = " << this->generationArgs.pixelSize << "\n";
	stringStream << "\t}\n";
	stringStream << "\tanalysis = {\n";
	stringStream << "\t\tparsed = " << this->analysisArgs.parsed << "\n";
	stringStream << "\t\tdirectory = '" << this->analysisArgs.directoryPath << "'\n";
	stringStream << "\t\trecursive = " << this->analysisArgs.recursive << "\n";
	stringStream << "\t}\n";
	stringStream << "}";
	return stringStream.str();
}