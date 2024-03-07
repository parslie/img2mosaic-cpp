#pragma once

#include <string>

struct GenerationArgs
{
	bool parsed = false;
	std::string srcPath = "";
	std::string dstPath = "";
	unsigned int srcSize = 128;
	unsigned int pixelSize = 16;
};

struct AnalysisArgs
{
	bool parsed = false;
	std::string dirPath = "";
	bool recursive = false;
};

struct Arguments
{
	std::string profile = "default";
	bool debug = false;

	GenerationArgs generation;
	AnalysisArgs analysis;

	std::string toString();
};

Arguments parseArgs(int argc, const char* const *argv);