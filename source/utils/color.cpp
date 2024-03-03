#include "color.hpp"

#include <sstream>

using namespace std;

string colorToString(const cv::Vec3b &color)
{
	stringstream sstream;
	sstream << (int)color[2] << "," << (int)color[1] << "," << (int)color[0];
	return sstream.str();
}

cv::Vec3b stringToColor(const string &colorString)
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
