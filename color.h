#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include <iostream>

// Define 'color' as an alias for 'vec3'
using color = vec3;

// Convert linear color to gamma space
inline double linear_to_gamma(double linear_component)
{
    // As a simple approximation use “gamma 2” as our transform, which is the power that you use when going from gamma space to linear space.
    // We need to go from linear space to gamma space, which means taking the inverse of “gamma 2", which means an exponent of 1/gamma, which is just the square - root.
    return sqrt(linear_component);
}

// Function to write the color to the output stream in PPM format
void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply the linear to gamma transform.
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Write the translated [0,255] value of each color component.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}


#endif