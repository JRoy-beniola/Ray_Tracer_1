//
// Created by royja on 21-04-2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera
{
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;

    void render(const hittable& world)
    {
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

            for (int i = 0; i < image_width; i++)
            {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }


private:
    int image_height;
    point3 camera_centre, first_pixel_loc;
    vec3 delta_u, delta_v;
    double pixel_samples_scale;

    void initialize()
    {
        //Defining image attributes
        image_height = image_width / aspect_ratio;
        image_height = (image_height < 1) ? 1 : image_height;

        // Scaling down sum of pixel colors to the avg
        pixel_samples_scale = 1.0 / samples_per_pixel;

        // Setting up the camera
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * ((double)image_width/image_height);
        camera_centre = point3(0, 0, 0);

        // Calculating vectors across the scenery. (These are spans only.)
        auto vector_u = vec3(viewport_width, 0, 0);
        auto vector_v = vec3(0, -viewport_height, 0);

        // Calculating inter pixel centre delta vectors in both direction
        delta_u = vector_u/image_width;
        delta_v = vector_v/image_height;

        // We will drop the camera so gotta calculate for that.
        auto extreme_upper_left = camera_centre
                                        - vec3(0, 0, focal_length)     /* The camera was dropped by its focal length flat onto the screen */
                                        - vector_u / 2                  /* The camera was moved left by half the span of vector_u. */
                                        - vector_v / 2;                 /* The camera was moved left by half the span of vector_v. */

        first_pixel_loc = extreme_upper_left + 0.5 * (delta_u + delta_v);
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = first_pixel_loc
                          + ((i + offset.x()) * delta_u)
                          + ((j + offset.y()) * delta_v);

        auto ray_origin = camera_centre;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray& r, const hittable& world) const
    {
        hit_record rec;

        if (world.hit(r, interval(0, infinity), rec))
        {
            vec3 direction = random_on_hemisphere(rec.normal);
            return 0.5 * ray_color(ray(rec.p, direction), world);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto alpha = 0.5 * (unit_direction.y() + 1.0);
        return (1 - alpha)*color(1.0, 1.0, 1.0) + alpha*color(0.5, 0.7, 1.0);

    }
};



#endif //CAMERA_H
