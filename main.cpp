#include <iostream>

#include "vec3.h"
#include "color.h"
#include "ray.h"


color ray_color(const ray&);


int main()
{
    // Scenery
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculating img height & making sure it's not < 1.
    int image_height = image_width / aspect_ratio;
    image_height = (image_height < 1) ? 1 : image_height;

    // Setting up the camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * ((double)image_width/image_height);
    auto camera_centre = point3(0, 0, 0);

    // Calculating vectors across the scenery. (These are spans only.)
    auto vector_u = vec3(viewport_width, 0, 0);
    auto vector_v = vec3(0, -viewport_height, 0);

    // Calculating inter pixel centre delta vectors in both direction
    auto delta_u = vector_u/image_width;
    auto delta_v = vector_v/image_height;

    // We will drop the camera so gotta calculate for that.
    auto extreme_upper_left = camera_centre
                                    - vec3(0, 0, -focal_length)     /* The camera was dropped by its focal length flat onto the screen */
                                    - vector_u / 2                  /* The camera was moved left by half the span of vector_u. */
                                    - vector_v / 2;                 /* The camera was moved left by half the span of vector_v. */

    auto first_pixel_loc = extreme_upper_left + 0.5 * (delta_u + delta_v);


    // Rendering shenanigans

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; i++)
        {
            auto pixel_centre = first_pixel_loc + (i * delta_u) + (j * delta_v);

            auto ray_direction = pixel_centre - camera_centre;

            ray r(camera_centre, ray_direction);

            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";

}


color ray_color(const ray& r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto alpha = 0.5 * (unit_direction.y() + 1.0);
    return (1 - alpha)*color(1.0, 1.0, 1.0) + alpha*color(0.5, 0.7, 1.0);
}
