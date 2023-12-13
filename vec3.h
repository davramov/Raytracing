#ifndef VEC3_H
#define VEC3_H

#include "helper.h"

#include <cmath>
#include <iostream>

using std::sqrt;

// 3D Vector Class
class vec3 {
public:
    // Components of the vector
    double e[3];

    // Constructors
    vec3() : e{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    // Accessors for vector components
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Unary Negation
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    // Indexing operators
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    // Compound assignment operators
    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    // Length of the vector
    double length() const {
        return sqrt(length_squared());
    }

    // Squared length of the vector
    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    // Generate arbitrary random vectors
    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

};

// Alias for vec3 used to represent points in 3D space
using point3 = vec3;

// Vector Utility Functions

// Output operator for vec3
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Vector addition
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Vector subtraction
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Element-wise multiplication
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Multiplication by a scalar from the left
inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

// Multiplication by a scalar from the right
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

// Division by a scalar
inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

// Dot product of two vectors
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// Cross product of two vectors
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Unit vector in the direction of a vector
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// Generate random vector inside of unit disk (for camera depth of field or defocus blur)
inline vec3 random_in_unit_disk() {
    while (true) {
        // Same logic as unit sphere, but 2 dimensions
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

// Generate random vector inside the unit sphere (for reflections)
inline vec3 random_in_unit_sphere() {
    while (true) {

        // Generate a random vector where each component is a value between -1 and 1
        auto p = vec3::random(-1, 1);

        // Look for a vector inside of the unit sphere
        if (p.length_squared() < 1)
            return p;
    }
}

// Get a random unit vector within the unit sphere (normalized)
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

// Determine if the random vector is in the correct hemisphere by computing a dot product with the surface normal vector
inline vec3 random_on_hemisphere(const vec3& normal) {
    // Get a random unit vector
    vec3 on_unit_sphere = random_unit_vector();
    // Compute dot product between random unit vector and the normal
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        // Flip the random vector if it is in the wrong hemisphere
        return -on_unit_sphere;
}

// Mirrored light reflection
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

// Refraction using snell's law for dielectric (transparent) materials
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    // uv is the incident ray
    // n is the normal vector
    // etai_over_etat is the ratio of refractive indices of the two mediums (incoming/outgoing)
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}




#endif