#include "color.hpp"

#include <sstream>

using namespace std;

string colorToString(const Color &color)
{
	stringstream sstream;
	sstream << (int)color[2] << "," << (int)color[1] << "," << (int)color[0];
	return sstream.str();
}

Color stringToColor(const string &colorString)
{
	size_t firstSplit = colorString.find(',');
	size_t secondSplit = colorString.find(',', firstSplit + 1);

	string redString = colorString.substr(0, firstSplit + 1);
	string greenString = colorString.substr(firstSplit + 1, secondSplit - firstSplit);
	string blueString = colorString.substr(secondSplit + 1);

	uchar red = stoi(redString);
	uchar green = stoi(greenString);
	uchar blue = stoi(blueString);

	return cv::Vec3b(blue, green, red);
}

double getColorDist(const Color &colorA, const Color &colorB)
{
	double distance = 0.0;

	double redDifference = (double)((int)(colorA[2]) - (int)(colorB[2]));
	double greenDifference = (double)((int)(colorA[1]) - (int)(colorB[1]));
	double blueDifference = (double)((int)(colorA[0]) - (int)(colorB[0]));

	distance += pow(redDifference, 2);
	distance += pow(greenDifference, 2);
	distance += pow(blueDifference, 2);

	return sqrt(distance);
}
