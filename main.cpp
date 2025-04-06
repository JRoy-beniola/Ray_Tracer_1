#include <iostream>

#include "color.h"
#include "vec3.h"

#define image_width 256
#define image_height 256


int main() {
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(double(i)/(image_width-1), 0, double(j)/(image_height-1));
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}