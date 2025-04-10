//
// Created by royja on 09-04-2025.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class sphere
{
    private:
        point3 centre;
        double radius;

    public:
        sphere(const point3& _centre, double _radius) : centre(_centre), radius(fmax(0, _radius)) {}

        bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override
        {
            auto oc = r.origin() - centre;
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius * radius;

            auto discriminant = h*h - a*c;

            if (discriminant < 0) return false;

            auto sqrt_discriminant = sqrt(discriminant);

            auto root = (h - sqrt_discriminant) / a;

            if (root < tmin || root > tmax)
            {
                root = (h + sqrt_discriminant) / a;

                if (root < tmin || root > tmax) return false;
            }

            rec.t = root;
            rec.p = r.at(root);
            rec.normal = (rec.p - centre) / radius;

            return true;
        }
};

#endif //SPHERE_H
