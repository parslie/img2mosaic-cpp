#include "image.hpp"

#include <iostream>

using namespace std;
namespace fs = std::filesystem;

Image::Image(uint width, uint height, fs::path path) {
	this->width = width;
	this->height = height;
	this->path = path;
	this->mat = cv::Mat::zeros((int)height, (int)width, CV_8UC3);
}

Image::Image(fs::path path) {
	this->path = path;
	this->mat = cv::imread(path.string(), cv::IMREAD_COLOR);
	this->width = (uint)(this->mat.cols);
	this->height = (uint)(this->mat.rows);
}

void Image::resize(uint new_width, uint new_height) {
	cv::Mat resized_mat;
	cv::Size new_size((int)new_width, (int)new_height);
	cv::resize(this->mat, resized_mat, new_size);
	this->mat = resized_mat;
	this->width = new_width;
	this->height = new_height;
}

void Image::scale(double factor) {
	uint new_width = (uint)ceil(factor * this->width);
	uint new_height = (uint)ceil(factor * this->height);
	this->resize(new_width, new_height);
}

double Image::contain_in_size(uint size) {
	double scale_factor = min(
		(double)size / this->width,
		(double)size / this->height
	);

	if (scale_factor < 1.0) {
		this->scale(scale_factor);
		return scale_factor;
	}

	return 1.0;
}

double Image::fit_to_size(uint size) {
	double scale_factor = max(
		(double)size / this->width,
		(double)size / this->height
	);

	if (scale_factor < 1.0) {
		this->scale(scale_factor);
		return scale_factor;
	}

	return 1.0;
}

Color &Image::at(uint x, uint y) {
	return this->mat.at<Color>((int)y, (int)x);
}

Color Image::average_color() {
	cv::Mat resized_mat;
	cv::Size new_size(16, 16);
	cv::resize(this->mat, resized_mat, new_size);
	// TODO: test if resizing to 1x1 gives average color

	double total_blue = 0.0, total_green = 0.0, total_red = 0.0;
	uint color_count = 16 * 16;

	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 16; x++) {
			Color color = resized_mat.at<Color>(y, x);
			total_blue += color[0];
			total_green += color[1];
			total_red += color[2];
		}
	}

	uchar average_blue = (uchar)round(total_blue / color_count);
	uchar average_green = (uchar)round(total_green / color_count);
	uchar average_red = (uchar)round(total_red / color_count);
	return Color(average_blue, average_green, average_red);
}

uint Image::get_width() const {
	return this->width;
}

uint Image::get_height() const {
	return this->height;
}

void Image::save() {
	cv::imwrite(this->path.string(), this->mat);
}

vector<ImageSection> Image::split() {
	if (this->width > this->height)
		return this->split_horizontal();
	else
		return this->split_vertical();
}

vector<ImageSection> Image::split_horizontal() const {
	uint section_size = min(this->width, this->height);
	double cols = (double)(this->width) / (double)section_size;

	vector<ImageSection> sections;
	for (uint col = 0; col < cols; col++) {
		uint x_min = section_size * col;
		uint x_max = section_size * (col + 1);
		uint y_min = 0;
		uint y_max = section_size;

		if (x_max > this->width) {
			uint diff = x_max - this->width;
			x_max -= diff;
			x_min -= diff;
		}

		ImageSection section = {
			this->path,
			x_min,
			y_min,
			section_size,
			section_size
		};
		sections.push_back(section);
	}

	return sections;
}

vector<ImageSection> Image::split_vertical() const {
	uint section_size = min(this->width, this->height);
	double rows = (double)(this->height) / (double)section_size;

	vector<ImageSection> sections;
	for (uint row = 0; row < rows; row++) {
		uint x_min = 0;
		uint x_max = section_size;
		uint y_min = section_size * row;
		uint y_max = section_size * (row + 1);

		if (y_max > this->height) {
			uint diff = y_max - this->height;
			y_max -= diff;
			y_min -= diff;
		}

		ImageSection section = {
			this->path,
			x_min,
			y_min,
			section_size,
			section_size
		};
		sections.push_back(section);
	}

	return sections;
}