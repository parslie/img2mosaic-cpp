#include "generation.hpp"
#include "../data/palette.hpp"
#include "../utils/color.hpp"

#include <string>

#include <opencv2/opencv.hpp>

using namespace std;

cv::Mat loadSourceImage(string path, unsigned int size)
{
	cv::Mat originalImage = cv::imread(path);

	double resizeFactor = min(
		(double)size / originalImage.rows,
		(double)size / originalImage.cols
	);

	if (resizeFactor < 1.0)
	{
		int resizedRows = round(originalImage.rows * resizeFactor);
		int resizedCols = round(originalImage.cols * resizeFactor);
		cv::Mat resizedImage;
		cv::resize(originalImage, resizedImage, cv::Size(resizedCols, resizedRows));
		return resizedImage;
	}
	else
	{
		return originalImage;
	}
}

void generation::run(const Arguments &arguments)
{
	// 1. Initialize palette
	Palette palette = loadPalette(arguments.profile);

	// 2. Load source image
	cv::Mat sourceImage = loadSourceImage(arguments.generationArgs.sourcePath, arguments.generationArgs.sourceSize);
	int sourceRows = sourceImage.rows;
	int sourceCols = sourceImage.cols;
	
	// 3. Initialize destination image
	int destinationRows = arguments.generationArgs.pixelSize * sourceRows;
	int destinationCols = arguments.generationArgs.pixelSize * sourceCols;
	cv::Mat destinationImage = cv::Mat::zeros(destinationRows, destinationCols, sourceImage.type());
	
	// 4. Iterate over each pixel in source image
	unsigned int pixelsInserted = 0;
	unsigned int pixelCount = sourceRows * sourceCols;
	for (int sourceCol = 0; sourceCol < sourceCols; sourceCol++)
	{
		for (int sourceRow = 0; sourceRow < sourceRows; sourceRow++)
		{
			cout << '\r' << pixelsInserted << '/' << pixelCount << " pixels inserted.";

			cv::Vec3b sourceColor = sourceImage.at<cv::Vec3b>(sourceRow, sourceCol);
			cv::Mat pixelImage = paletteGetImage(palette, sourceColor, arguments.generationArgs.pixelSize);
			
			for (int pixelCol = 0; pixelCol < pixelImage.cols; pixelCol++)
			{
				for (int pixelRow = 0; pixelRow < pixelImage.rows; pixelRow++)
				{
					cv::Vec3b color = pixelImage.at<cv::Vec3b>(pixelRow, pixelCol);
					int destinationRow = sourceRow * pixelImage.rows + pixelRow;
					int destinationCol = sourceCol * pixelImage.cols + pixelCol;
					destinationImage.at<cv::Vec3b>(destinationRow, destinationCol) = color;
				}
			}

			pixelsInserted++;
		}
	}
	cout << '\r' << pixelCount << '/' << pixelCount << " pixels inserted.";

	// 5. Save destination image
	cv::imwrite(arguments.generationArgs.destinationPath, destinationImage);
}