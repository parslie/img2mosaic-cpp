#include "commands.hpp"

#include "../data/image.hpp"
#include "../data/palette.hpp"
#include "../utils/color.hpp"
#include "../utils/progress.hpp"

#include <iostream>

int generate(const Arguments &args)
{
    constexpr const char *TAB{ "   " };

    std::cout << "[Preparing generation...]" << '\n';
    std::shared_ptr<ImageLoader> img_loader{ std::make_shared<ImageLoader>() };
    Palette palette{ args.profile, img_loader };
    std::cout << TAB << "Loaded palette." << '\n';

    Image src_img{ args.generation_args.src_path };
    src_img.scale_to_cover(args.generation_args.src_size);
    std::cout << TAB << "Loaded source image." << '\n';

    unsigned int dst_width{ src_img.width() * args.generation_args.pixel_size };
    unsigned int dst_height{ src_img.height() * args.generation_args.pixel_size };
    Image dst_img{ dst_width, dst_height, args.generation_args.dst_path };
    std::cout << TAB << "Loaded destination image." << '\n';

    std::cout << "[Starting generation...]" << '\n';
    Progress progress{ src_img.width() * src_img.height() };
    for (unsigned int src_y{ 0 }; src_y < src_img.height(); ++src_y)
    {
        for (unsigned int src_x{ 0 }; src_x < src_img.width(); ++src_x)
        {
            std::cout << TAB << progress.to_string() << '\r';

            const ColorBGR &color{ src_img.at(src_x, src_y) };
            Image color_img = palette.at(color, args.generation_args.pixel_size);

            unsigned int dst_x{ src_x * args.generation_args.pixel_size };
            unsigned int dst_y{ src_y * args.generation_args.pixel_size };
            dst_img.insert(dst_x, dst_y, color_img);

            progress.increment();
        }
    }
    std::cout << TAB << progress.to_string() << '\n';

    std::cout << "[Saving mosaic...]" << '\n';
    dst_img.save();
    std::cout << TAB << "Saved mosaic to \"" << args.generation_args.dst_path.string() << "\"." << '\n';

    return EXIT_SUCCESS;
}
