#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

#include "helper.h"

class material; // This will be defined later


// Class representing a hit record with information about a ray-object intersection
class hit_record {
public:
    point3 p;                   // Point of intersection
    vec3 normal;                // Normal vector at the point of intersection
    shared_ptr<material> mat;   // Material pointer
    double t;                   // Parameter 't' representing the distance along the ray to the point of intersection


    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }

};

// Abstract base class for hittable objects, defining an interface for ray-object intersection
class hittable {
public:
    // Virtual destructor for proper cleanup of derived classes
    virtual ~hittable() = default;

    // Function to check if a ray hits the object within a specified range and update the hit record if a hit occurs
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif
