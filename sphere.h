//
// Created by royja on 09-04-2025.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
    private:
        point3 centre;
        double radius;

    public:
        sphere(const point3& _centre, double _radius) : centre(_centre), radius(fmax(0, _radius)) {}

        bool hit(const ray& r, interval ray_inter, hit_record& rec) const override
        {
            auto oc = centre - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius * radius;

            auto discriminant = h*h - a*c;

            if (discriminant < 0) return false;

            auto sqrt_discriminant = std::sqrt(discriminant);

            auto root = (h - sqrt_discriminant) / a;

            if (!ray_inter.surrounds(root))
            {
                root = (h + sqrt_discriminant) / a;

                if (!ray_inter.surrounds(root)) return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - centre) / radius;

            vec3 outward_normal = (rec.p - centre) / radius;
            rec.set_face_normal(r, outward_normal);

            return true;
        }
};

#endif //SPHERE_H
