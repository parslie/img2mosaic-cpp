#include "argument.hpp"
#include "CLI/CLI.hpp"

// TODO: add validators

static CLI::App *create_generation_subapp(CLI::App &app, GenerationArgs &args) {
	CLI::App *subapp = app.add_subcommand(
		"generate",
		"Generate a mosaic"
	);

	subapp->add_option<std::string>(
		"source",
		args.src_path,
		"Path to the image to mosaic"
	)->option_text("PATH")->required(true);

	subapp->add_option<std::string>(
		"destination",
		args.dst_path,
		"Path to output the mosaic to"
	)->option_text("PATH")->required(true);

	subapp->add_option<unsigned int>(
		"-s,--source-size",
		args.src_size,
		"The size of the image to mosaic"
	)->option_text("PIXELS");

	subapp->add_option<unsigned int>(
		"-p,--pixel-size",
		args.pixel_size,
		"The size of the images to use as pixels"
	)->option_text("PIXELS");

	return subapp;
}

static CLI::App *create_analysis_subapp(CLI::App &app, AnalysisArgs &args) {
	CLI::App *subapp = app.add_subcommand(
		"analyze",
		"Analyze images to represent pixels"
	);

	subapp->add_option<std::string>(
		"directory",
		args.dir_path,
		"The directory containing the images to analyze"
	)->option_text("PATH")->required(true);

	subapp->add_flag<bool>(
		"-r,--recursive",
		args.recursive,
		"Analyze images in subdirectories as well"
	);

	return subapp;
}

Arguments parse_argv(const int argc, const char *const *argv) {
	Arguments args;

	CLI::App app(
		"A program that generates mosaics of images made of other images",
		"img2mosaic"
	);

	app.add_option<std::string>(
		"profile",
		args.profile,
		"The profile to use"
	)->option_text("TEXT");

	app.add_flag<bool>(
		"--debug",
		args.debug,
		"Print debug statements"
	);

	CLI::App *generation = create_generation_subapp(app, args.generation);
	CLI::App *analysis = create_analysis_subapp(app, args.analysis);
	
	try {
		app.parse(argc, argv);
	} catch (const CLI::ParseError &e) {
		exit(app.exit(e));
	}

	args.generation.parsed = generation->parsed();
	args.analysis.parsed = analysis->parsed();

	return args;
}

std::string Arguments::to_string() const {
	std::stringstream stringStream;
	stringStream << "{\n";
	stringStream << "\tprofile = '" << this->profile << "'\n";
	stringStream << "\tdebug = " << this->debug << "\n";
	stringStream << "\tgeneration = {\n";
	stringStream << "\t\tparsed = " << this->generation.parsed << "\n";
	stringStream << "\t\tsource = '" << this->generation.src_path << "'\n";
	stringStream << "\t\tdestination = '" << this->generation.dst_path << "'\n";
	stringStream << "\t\tsource size = " << this->generation.src_size << "\n";
	stringStream << "\t\tpixel size = " << this->generation.pixel_size << "\n";
	stringStream << "\t}\n";
	stringStream << "\tanalysis = {\n";
	stringStream << "\t\tparsed = " << this->analysis.parsed << "\n";
	stringStream << "\t\tdirectory = '" << this->analysis.dir_path << "'\n";
	stringStream << "\t\trecursive = " << this->analysis.recursive << "\n";
	stringStream << "\t}\n";
	stringStream << "}";
	return stringStream.str();
}
