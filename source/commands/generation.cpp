#include "generation.hpp"
#include "../data/palette.hpp"
#include "../utils/image.hpp"
#include "../utils/color.hpp"

#include <string>

#include <opencv2/opencv.hpp>

using namespace std;

void generateImg(const Arguments &args)
{
	// 1. Initialize palette
	Palette palette = loadPalette(args.profile);

	// 2. Load source image
	Image srcImg(args.generation.srcPath);
	srcImg.containInSize(args.generation.srcSize);
	
	// 3. Initialize destination image
	int dstHeight = args.generation.pixelSize * srcImg.getHeight();
	int dstWidth = args.generation.pixelSize * srcImg.getWidth();
	Image dstImg(dstWidth, dstHeight, args.generation.dstPath);
	
	// 4. Iterate over each pixel in source image
	uint pixelsInserted = 0;
	uint pixelCount = srcImg.getWidth() * srcImg.getHeight();

	for (uint srcY = 0; srcY < srcImg.getHeight(); srcY++)
	{
		for (uint srcX = 0; srcX < srcImg.getWidth(); srcX++)
		{
			cout << '\r' << pixelsInserted << '/' << pixelCount << " pixels inserted.";

			Image pixelImg = paletteGetImg(palette, srcImg.at(srcX, srcY), args.generation.pixelSize);
			for (uint pixelY = 0; pixelY < pixelImg.getHeight(); pixelY++)
			{
				for (uint pixelX = 0; pixelX < pixelImg.getWidth(); pixelX++)
				{
					uint dstX = srcX * pixelImg.getWidth() + pixelX;
					uint dstY = srcY * pixelImg.getHeight() + pixelY;
					dstImg.at(dstX, dstY) = pixelImg.at(pixelX, pixelY);
				}
			}

			pixelsInserted++;
		}
	}
	cout << '\r' << pixelCount << '/' << pixelCount << " pixels inserted.";

	// 5. Save destination image
	dstImg.save();
}