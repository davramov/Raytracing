#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"

// Class representing a sphere as a hittable object
class sphere : public hittable {
public:
    // Constructor taking center, radius, and material pointer parameters to define the sphere
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
        : center(_center), radius(_radius), mat(_material) {}

    // Function to check if a ray hits the sphere within a specified range
    // and update the hit record if a hit occurs
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Compute the direction vector from the ray's origin to the sphere's center
        vec3 oc = r.origin() - center;

        // Coefficients of the quadratic equation for ray-sphere intersection
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        // Discriminant of the quadratic equation
        auto discriminant = half_b * half_b - a * c;

        // Check if the ray intersects the sphere
        if (discriminant < 0)
            return false;

        // Compute the roots of the quadratic equation
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        // Update the hit record with intersection details
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;


        return true;
    }

private:
    // Center and radius of the sphere
    point3 center;
    double radius;

    // Material pointer
    shared_ptr<material> mat;
};

#endif
