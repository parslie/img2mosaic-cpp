#include "image.hpp"

#include <iostream>

using namespace std;
namespace fs = std::filesystem;

static vector<ImageSection> splitImageVertical(const fs::path &path, const cv::Mat &image, int sectionSize)
{
	int height = image.size[0];
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

		cv::Mat sectionMat = cv::Mat::zeros(sectionSize, sectionSize, image.type());
		for (int y = yMin; y < yMax; y++)
		{
			for (int x = xMin; x < xMax; x++)
			{
				cv::Vec3b bgrPixel = image.at<cv::Vec3b>(y, x);
				sectionMat.at<cv::Vec3b>(y - yMin, x - xMin) = bgrPixel;
			}
		}

		ImageSection section = {
			sectionMat,
			path,
			xMin,
			yMax,
			sectionSize,
			sectionSize
		};
		sections.push_back(section);
	}

	return sections;
}

static vector<ImageSection> splitImageHorizontal(const fs::path &path, const cv::Mat &image, int sectionSize)
{
	int width = image.size[1];
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

		cv::Mat sectionMat = cv::Mat::zeros(sectionSize, sectionSize, image.type());
		for (int y = yMin; y < yMax; y++)
		{
			for (int x = xMin; x < xMax; x++)
			{
				cv::Vec3b bgrPixel = image.at<cv::Vec3b>(y, x);
				sectionMat.at<cv::Vec3b>(y - yMin, x - xMin) = bgrPixel;
			}
		}

		ImageSection section = {
			sectionMat,
			path,
			xMin,
			yMax,
			sectionSize,
			sectionSize
		};
		sections.push_back(section);
	}

	return sections;
}

vector<ImageSection> splitImage(const fs::path &path)
{
	cv::Mat image = cv::imread(path.string(), cv::IMREAD_COLOR);

	if (image.size[0] > image.size[1])
	{
		cout << "Vertical (" << image.size[1] << "x" << image.size[0] << ")" << endl;
		return splitImageVertical(path, image, image.size[1]);
	}
	else
	{
		cout << "Horizontal (" << image.size[1] << "x" << image.size[0] << ")" << endl;
		return splitImageHorizontal(path, image, image.size[0]);
	}
}

cv::Vec3b getAverageColor(const cv::Mat &mat)
{
	cv::Mat resizedMat;
	cv::resize(mat, resizedMat, cv::Size(16, 16));

	double totalBlue = 0.0, totalGreen = 0.0, totalRed = 0.0;
	int colorCount = 16 * 16;

	for (int y = 0; y < resizedMat.size[0]; y++)
	{
		for (int x = 0; x < resizedMat.size[1]; x++)
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
