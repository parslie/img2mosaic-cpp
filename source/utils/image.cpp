#include "image.hpp"

#include <iostream>

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

static vector<ImageSection> splitImgVertical(const fs::path &path, const cv::Mat &img, int sectionSize)
{
	int height = img.size[0];
	double rows = (double)height / (double)sectionSize;

	vector<ImageSection> sections;

	for (int row = 0; row < rows; row++)
	{
		int xMin = 0;
		int xMax = sectionSize;
		int yMin = sectionSize * row;
		int yMax = sectionSize * (row + 1);

		if (yMax > height)
		{
			int diff = yMax - height;
			yMax -= diff;
			yMin -= diff;
		}

		ImageSection section = {
			path,
			xMin,
			yMin,
			sectionSize,
			sectionSize
		};
		sections.push_back(section);
	}

	return sections;
}

static vector<ImageSection> splitImgHorizontal(const fs::path &path, const cv::Mat &img, int sectionSize)
{
	int width = img.size[1];
	double cols = (double)width / (double)sectionSize;

	vector<ImageSection> sections;

	for (int col = 0; col < cols; col++)
	{
		int xMin = sectionSize * col;
		int xMax = sectionSize * (col + 1);
		int yMin = 0;
		int yMax = sectionSize;

		if (xMax > width)
		{
			int diff = xMax - width;
			xMax -= diff;
			xMin -= diff;
		}

		ImageSection section = {
			path,
			xMin,
			yMin,
			sectionSize,
			sectionSize
		};
		sections.push_back(section);
	}

	return sections;
}

vector<ImageSection> splitImg(const fs::path &path)
{
	cv::Mat img = cv::imread(path.string(), cv::IMREAD_COLOR);

	if (img.size[0] > img.size[1])
		return splitImgVertical(path, img, img.size[1]);
	else
		return splitImgHorizontal(path, img, img.size[0]);
}

cv::Vec3b getAverageColor(const cv::Mat &img)
{
	cv::Mat resizedImg;
	cv::resize(img, resizedImg, cv::Size(16, 16));

	double totalBlue = 0.0, totalGreen = 0.0, totalRed = 0.0;
	int colorCount = 16 * 16;

	for (int y = 0; y < resizedImg.size[0]; y++)
	{
		for (int x = 0; x < resizedImg.size[1]; x++)
		{
			cv::Vec3b color = resizedImg.at<cv::Vec3b>(y, x);
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

cv::Mat imreadImgSection(const ImageSection &imgSection)
{
	cv::Mat fullImg = cv::imread(imgSection.path.string(), cv::IMREAD_COLOR);
	cv::Mat sectionImg = cv::Mat::zeros(imgSection.height, imgSection.width, fullImg.type());

	for (int y = imgSection.y; y < imgSection.y + imgSection.height; y++)
	{
		for (int x = imgSection.x; x < imgSection.x + imgSection.width; x++)
		{
			cv::Vec3b color = fullImg.at<cv::Vec3b>(y, x);
			sectionImg.at<cv::Vec3b>(y - imgSection.y, x - imgSection.x) = color;
		}
	}

	return sectionImg;
}

nlohmann::json ImageSection::toJson() const
{
	json data = json::object();
	data["path"] = this->path.string();
	data["x"] = this->x;
	data["y"] = this->y;
	data["width"] = this->width;
	data["height"] = this->height;
	return data;
}
