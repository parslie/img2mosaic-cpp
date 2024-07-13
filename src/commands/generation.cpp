#include "commands.hpp"

#include "../data/image.hpp"
#include "../data/palette.hpp"

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
    src_img.show("src");
    dst_img.show("dst");

    std::cout << "[Saving mosaic...]" << '\n';

    return EXIT_SUCCESS;
}
