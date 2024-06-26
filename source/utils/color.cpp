#include "color.hpp"

#include <sstream>

using namespace std;

string color_to_string(const Color &color) {
	stringstream sstream;
	sstream << (int)color[2] << "," << (int)color[1] << "," << (int)color[0];
	return sstream.str();
}

Color string_to_color(const string &color_string) {
	size_t first_split = color_string.find(',');
	size_t second_split = color_string.find(',', first_split + 1);

	string red_string = color_string.substr(0, first_split + 1);
	string green_string = color_string.substr(first_split + 1, second_split - first_split);
	string blue_string = color_string.substr(second_split + 1);

	uchar red = stoi(red_string);
	uchar green = stoi(green_string);
	uchar blue = stoi(blue_string);
	return cv::Vec3b(blue, green, red);
}

double get_color_dist(const Color &color_a, const Color &color_b) {
	double distance = 0.0;

	double red_difference = (double)((int)(color_a[2]) - (int)(color_b[2]));
	double green_difference = (double)((int)(color_a[1]) - (int)(color_b[1]));
	double blue_difference = (double)((int)(color_a[0]) - (int)(color_b[0]));

	distance += pow(red_difference, 2);
	distance += pow(green_difference, 2);
	distance += pow(blue_difference, 2);
	return sqrt(distance);
}
