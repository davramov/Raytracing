#include "helper.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
    hittable_list world;

    // Add metal spheres to the scene

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<dielectric>(1.5);
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0); // second parameter is fuzz
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.7);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), -0.4, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));


    // Set up the camera's public variables
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 40;
    cam.max_depth = 10;
    cam.vfov = 40; // Smaller values are more zoomed in/cropped

    // Position and direction
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    // DoF
    cam.defocus_angle = 4.0; // Low values leads to less defocus blur
    cam.focus_dist = 3.4; // The focus distance should match the distance from the camera to the object of interest

    cam.render(world);
}
