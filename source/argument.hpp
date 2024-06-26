#pragma once

#include <string>

struct GenerationArgs {
	bool parsed = false;

	std::string src_path = "";
	std::string dst_path = "";
	unsigned int src_size = 128;
	unsigned int pixel_size = 16;
};

struct AnalysisArgs {
	bool parsed = false;

	std::string dir_path = "";
	bool recursive = false;
};

struct Arguments {
	GenerationArgs generation;
	AnalysisArgs analysis;

	std::string profile = "default";
	bool debug = false;

	std::string to_string() const;
};

Arguments parse_argv(int argc, const char *const *argv);
