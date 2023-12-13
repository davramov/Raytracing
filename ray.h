#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// Class representing a ray in 3D space
class ray {
public:
    // Constructors
    ray() {}

    // Construct a ray with given origin and direction
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    // Accessors for ray properties
    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    // Compute the point on the ray at parameter t
    point3 at(double t) const {
        return orig + t * dir;
    }

private:
    point3 orig; // Origin point of the ray
    vec3 dir;    // Direction vector of the ray
};

#endif
