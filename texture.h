#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "helper.h"
#include "perlin.h"
#include "rtw_stb_image.h"

// Abstract base class for textures
class texture {
public:
    virtual ~texture() = default;

    // Function to retrieve the color value at given texture coordinates (u, v) and point in space (p)
    virtual color value(double u, double v, const point3& p) const = 0;
};

// Solid color texture class
class solid_color : public texture {
public:
    // Constructor taking a color value
    solid_color(color c) : color_value(c) {}

    // Constructor taking individual RGB values
    solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

    // Function to retrieve the color value for a solid color texture
    color value(double u, double v, const point3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

// Checker texture class
class checker_texture : public texture {
public:
    // Constructor with scale, even texture, and odd texture
    checker_texture(double _scale, shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

    // Constructor with scale, and two solid colors for even and odd regions
    checker_texture(double _scale, color c1, color c2)
        : inv_scale(1.0 / _scale),
        even(make_shared<solid_color>(c1)),
        odd(make_shared<solid_color>(c2))
    {}

    // Function to retrieve the color value for a checker texture
    color value(double u, double v, const point3& p) const override {
        auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

// Image texture class
class image_texture : public texture {
public:
    // Constructor taking a filename for the image
    image_texture(const char* filename) : image(filename) {}

    // Function to retrieve the color value for an image texture
    color value(double u, double v, const point3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return color(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = interval(0, 1).clamp(u);
        v = 1.0 - interval(0, 1).clamp(v);  // Flip V to image coordinates

        auto i = static_cast<int>(u * image.width());
        auto j = static_cast<int>(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    rtw_image image;
};

// Perlin noise texture class
class noise_texture : public texture {
public:

    noise_texture(double sc = 1.0, const color& c = color(1.0,1.0,1.0)) : scale(sc), color_value(c) {}

    color value(double u, double v, const point3& p) const override {
        auto s = scale * p;
        return color_value * 0.5 * (1 + sin(s.z() + 10 * noise.turb(s)));
    }

private:
    perlin noise;
    double scale;
    color color_value;
};

#endif