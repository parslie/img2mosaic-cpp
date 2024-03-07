#include "generation.hpp"
#include "../data/palette.hpp"
#include "../utils/color.hpp"

#include <string>

#include <opencv2/opencv.hpp>

using namespace std;

cv::Mat loadSourceImg(string path, unsigned int size)
{
	cv::Mat originalImg = cv::imread(path);

	double resizeFactor = min(
		(double)size / originalImg.rows,
		(double)size / originalImg.cols
	);

	if (resizeFactor < 1.0)
	{
		int resizedRows = round(originalImg.rows * resizeFactor);
		int resizedCols = round(originalImg.cols * resizeFactor);
		cv::Mat resizedImg;
		cv::resize(originalImg, resizedImg, cv::Size(resizedCols, resizedRows));
		return resizedImg;
	}
	else
	{
		return originalImg;
	}
}

void generateImg(const Arguments &args)
{
	// 1. Initialize palette
	Palette palette = loadPalette(args.profile);

	// 2. Load source image
	cv::Mat srcImg = loadSourceImg(args.generation.srcPath, args.generation.srcSize);
	int srcRows = srcImg.rows;
	int srcCols = srcImg.cols;
	
	// 3. Initialize destination image
	int destRows = args.generation.pixelSize * srcRows;
	int destCols = args.generation.pixelSize * srcCols;
	cv::Mat destImg = cv::Mat::zeros(destRows, destCols, srcImg.type());
	
	// 4. Iterate over each pixel in source image
	unsigned int pixelsInserted = 0;
	unsigned int pixelCount = srcRows * srcCols;
	for (int srcCol = 0; srcCol < srcCols; srcCol++)
	{
		for (int srcRow = 0; srcRow < srcRows; srcRow++)
		{
			cout << '\r' << pixelsInserted << '/' << pixelCount << " pixels inserted.";

			cv::Vec3b srcColor = srcImg.at<cv::Vec3b>(srcRow, srcCol);
			cv::Mat pixelImg = paletteGetImg(palette, srcColor, args.generation.pixelSize);
			
			for (int pixelCol = 0; pixelCol < pixelImg.cols; pixelCol++)
			{
				for (int pixelRow = 0; pixelRow < pixelImg.rows; pixelRow++)
				{
					cv::Vec3b color = pixelImg.at<cv::Vec3b>(pixelRow, pixelCol);
					int destRow = srcRow * pixelImg.rows + pixelRow;
					int destCol = srcCol * pixelImg.cols + pixelCol;
					destImg.at<cv::Vec3b>(destRow, destCol) = color;
				}
			}

			pixelsInserted++;
		}
	}
	cout << '\r' << pixelCount << '/' << pixelCount << " pixels inserted.";

	// 5. Save destination image
	cv::imwrite(args.generation.dstPath, destImg);
}