#include "generation.hpp"
#include "../data/palette.hpp"
#include <iostream>

using namespace std;

void generate_image(const Arguments &args) {
	cout << "[Loading palette...]" << endl;
	Palette palette = load_palette(args.profile);

	cout << "[Loading source image...]" << endl;
	Image src_image(args.generation.src_path);
	src_image.contain_in_size(args.generation.src_size);

	cout << "[Initializing destination image...]" << endl;
	uint dst_height = args.generation.pixel_size * src_image.get_height();
	uint dst_width= args.generation.pixel_size * src_image.get_width();
	Image dst_image(dst_width, dst_height, args.generation.dst_path);

	cout << "[Inserting pixel images...]" << endl;
	uint pixels_inserted = 0;
	uint pixel_count = src_image.get_width() * src_image.get_height();
	for (uint src_y = 0; src_y < src_image.get_height(); src_y++) {
		for (uint src_x = 0; src_x < src_image.get_width(); src_x++) {
			cout << '\r' << pixels_inserted << '/' << pixel_count << " pixels inserted.";

			Color pixel_color = src_image.at(src_x, src_y);
			Image pixel_image = palette_get_img(palette, pixel_color, args.generation.pixel_size);

			for (uint pixel_y = 0; pixel_y < pixel_image.get_height(); pixel_y++) {
				for (uint pixel_x = 0; pixel_x < pixel_image.get_width(); pixel_x++) {
					uint dst_x = src_x * pixel_image.get_width() + pixel_x;
					uint dst_y = src_y * pixel_image.get_height() + pixel_y;
					dst_image.at(dst_x, dst_y) = pixel_image.at(pixel_x, pixel_y);
				}
			}

			pixels_inserted++;
		}
	}
	cout << '\r' << pixels_inserted << '/' << pixel_count << " pixels inserted." << endl;

	cout << "[Saving destination image...]" << endl;
	dst_image.save();
}
