//
// Created by royja on 09-04-2025.
//

#ifndef HITTABLE_H
#define HITTABLE_H
#include "ray.h"
#include "vec3.h"

class hit_record
{
    public:
        point3 p;
        double t;
        vec3 normal;
};

class hittable
{
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif //HITTABLE_H
