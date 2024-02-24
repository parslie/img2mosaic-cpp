#pragma once

#include <string>

struct GenerationArgs
{
	bool parsed = false;
	std::string sourcePath = "";
	std::string destinationPath = "";
	unsigned int sourceSize = 128;
	unsigned int pixelSize = 16;
};

struct AnalysisArgs
{
	bool parsed = false;
	std::string directoryPath = "";
	bool recursive = false;
};

struct Arguments
{
	std::string profile = "default";
	bool debug = false;

	GenerationArgs generationArgs;
	AnalysisArgs analysisArgs;

	std::string toString();
};

Arguments parseArguments(int argc, const char* const *argv);