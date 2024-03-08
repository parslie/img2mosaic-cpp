#include "image.hpp"

#include <iostream>

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

// Image

Image::Image(uint width, uint height, fs::path path)
{
	this->path = path;
	this->width = width;
	this->height = height;
	this->mat = cv::Mat::zeros((int)height, (int)width, CV_8UC3);
}

Image::Image(fs::path path)
{
	this->path = path;
	this->mat = cv::imread(path.string());
	this->width = (uint)(this->mat.cols);
	this->height = (uint)(this->mat.rows);
}

void Image::resize(uint width, uint height)
{
	cv::Mat resizedMat;
	cv::resize(this->mat, resizedMat, cv::Size((int)width, (int)height));
	this->mat = resizedMat;
	this->width = width;
	this->height = height;
}

double Image::containInSize(uint size)
{
	double resizeFactor = min(
		(double)size / this->width,
		(double)size / this->height
	);

	if (resizeFactor < 1.0)
	{
		uint width = (uint)ceil(resizeFactor * this->width);
		uint height = (uint)ceil(resizeFactor * this->height);

		cv::Mat resizedMat;
		cv::resize(this->mat, resizedMat, cv::Size((int)width, (int)height));
		this->mat = resizedMat;
		this->width = width;
		this->height = height;

		return resizeFactor;
	}

	return 1.0;
}

double Image::fitToSize(uint size)
{
	double resizeFactor = max(
		(double)size / this->width,
		(double)size / this->height
	);

	if (resizeFactor < 1.0)
	{
		uint width = (uint)ceil(resizeFactor * this->width);
		uint height = (uint)ceil(resizeFactor * this->height);

		cv::Mat resizedMat;
		cv::resize(this->mat, resizedMat, cv::Size((int)width, (int)height));
		this->mat = resizedMat;
		this->width = width;
		this->height = height;

		return resizeFactor;
	}

	return 1.0;
}

Color &Image::at(uint x, uint y)
{
	return this->mat.at<Color>((int)y, (int)x);
}

Color Image::averageColor()
{
	cv::Mat resizedMat;
	cv::resize(this->mat, resizedMat, cv::Size(16, 16), 0.0, 0.0, cv::INTER_CUBIC);
	// TODO: test if resizing to 1x1 gives average color

	double totalBlue = 0.0, totalGreen = 0.0, totalRed = 0.0;
	uint colorCount = 16 * 16;

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			cv::Vec3b color = resizedMat.at<cv::Vec3b>(y, x);
			totalBlue += color[0];
			totalGreen += color[1];
			totalRed += color[2];
		}
	}

	uchar averageBlue = (uchar)round(totalBlue / colorCount);
	uchar averageGreen = (uchar)round(totalGreen / colorCount);
	uchar averageRed = (uchar)round(totalRed / colorCount);
	return cv::Vec3b(averageBlue, averageGreen, averageRed);
}

std::vector<ImageSection> Image::split()
{
	if (this->width > this->height)
		return this->splitHorizontal();
	else
		return this->splitVertical();
}

uint Image::getWidth() const
{
	return this->width;
}

uint Image::getHeight() const
{
	return this->height;
}

void Image::save()
{
	cv::imwrite(this->path.string(), this->mat);
}

std::vector<ImageSection> Image::splitHorizontal() const
{
	uint sectionSize = min(this->width, this->height);
	double cols = (double)(this->width) / (double)sectionSize;

	vector<ImageSection> sections;
	for (uint col = 0; col < cols; col++)
	{
		uint xMin = sectionSize * col;
		uint xMax = sectionSize * (col + 1);
		uint yMin = 0;
		uint yMax = sectionSize;

		if (xMax > this->width)
		{
			uint diff = xMax - this->width;
			xMax -= diff;
			xMin -= diff;
		}

		ImageSection section = {
			this->path,
			xMin,
			yMin,
			sectionSize,
			sectionSize
		};
		sections.push_back(section);
	}
	return sections;
}

std::vector<ImageSection> Image::splitVertical() const
{
	uint sectionSize = min(this->width, this->height);
	double rows = (double)(this->height) / (double)sectionSize;

	vector<ImageSection> sections;
	for (uint row = 0; row < rows; row++)
	{
		uint xMin = 0;
		uint xMax = sectionSize;
		uint yMin = sectionSize * row;
		uint yMax = sectionSize * (row + 1);

		if (yMax > this->height)
		{
			uint diff = yMax - this->height;
			yMax -= diff;
			yMin -= diff;
		}

		ImageSection section = {
			this->path,
			xMin,
			yMin,
			sectionSize,
			sectionSize
		};
		sections.push_back(section);
	}
	return sections;
}

// ImageSection

nlohmann::json ImageSection::toJson() const
{
	json data = json::object();
	data["path"] = this->path.string();
	data["x"] = (int)(this->x);
	data["y"] = (int)(this->y);
	data["width"] = (int)(this->width);
	data["height"] = (int)(this->height);
	return data;
}

Image ImageSection::toImage(uint size)
{
	Image fullImg(this->path);

	double scaleFactor;
	if (size == 0.0)
	{
		size = min(fullImg.getWidth(), fullImg.getHeight());
		scaleFactor = 1.0;
	}
	else
	{
		scaleFactor = fullImg.fitToSize(size);
	}

	fs::path sectionPath = this->path.stem();
	sectionPath += "_section";
	sectionPath += this->path.extension();
	Image sectionImg(size, size, this->path);

	uint xMin = (uint)floor(this->x * scaleFactor);
	uint xMax = (uint)floor((this->x + this->width) * scaleFactor);
	uint yMin = (uint)floor(this->y * scaleFactor);
	uint yMax = (uint)floor((this->y + this->height) * scaleFactor);

	for (uint y = yMin; y < yMax; y++)
	{
		for (uint x = xMin; x < xMax; x++)
		{
			uint sectionX = x - xMin;
			uint sectionY = y - yMin;
			sectionImg.at(sectionX, sectionY) = fullImg.at(x, y);
		}
	}

	return sectionImg;
}
