#include "helper.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "bvh.h"
#include "texture.h"
#include "quad.h"
#include "interval.h"
#include "constant_medium.h"


void scene1() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    auto pertext = make_shared<noise_texture>(4, color(0.8, 0.6, 0.2));
    auto pertext1 = make_shared<noise_texture>(4, color(0.8, 0.1, 0.7));



    // Add metal spheres to the scene

    auto ground_material = make_shared<metal>(checker, 0.01);
    auto material_center = make_shared<dielectric>(1.5);
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0); // second parameter is fuzz
    auto material_right = make_shared<metal>(pertext, 0.2);
    auto quad_material = make_shared<metal>(color(0.7, 0.2, 0.4), 0.1);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, ground_material));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), -0.4, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    //world.add(make_shared<quad>(point3(-1, -1, -2), vec3(8, 0, 0), vec3(0, 4, 0), quad_material));

    auto difflight = make_shared<diffuse_light>(color(2, 2, 2));

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 110, difflight));
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 105, material_center));
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), -104, material_center));
    
    auto atmosphere = make_shared<sphere>(point3(0.0, 0.0, -1.0), 104, material_center);
    world.add(atmosphere);
    world.add(make_shared<constant_medium>(atmosphere, 0.015, color(0.2,0.3,0.7)));

    world.add(make_shared<sphere>(point3(-2.0, 0.0, 1.0), 0.4, material_center));
    world.add(make_shared<sphere>(point3(-2.0, 0.0, 1.0), -0.35, material_center));

    auto red = make_shared<lambertian>(pertext1);
    auto crystal_ball = make_shared<sphere>(point3(-2.0, 0.0, 1.0), 0.35, red);
    world.add(crystal_ball);
    world.add(make_shared<constant_medium>(crystal_ball, 10.0, pertext1));



    //world.add(make_shared<sphere>(point3(0.0, 10.0, -1.0), 9, difflight_g));
    //world.add(make_shared<sphere>(point3(-5.0, 10.0, -1.0), 9, difflight_b));
    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(1, 1, 1), quad_material);
    box1 = make_shared<rotate_x>(box1, -45);
    box1 = make_shared<rotate_z>(box1, -45);
    box1 = make_shared<rotate_y>(box1, 30);
    box1 = make_shared<translate>(box1, vec3(-0.6, 1.2, -0.6));
    //world.add(make_shared<constant_medium>(box1, 1, color(0.2, 0.3, 0.5)));

    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(0.5, 0.5, 0.5), quad_material);
    box2 = make_shared<translate>(box2, vec3(-0.5, -0.5, -3));
    box2 = make_shared<rotate_y>(box2, -30);
    world.add(box2);



    //world.add(box(point3(-2, -1, -2), point3(2.5, 2, -2.1), quad_material));


    //world.add(make_shared<cube>(point3(0.0, 1.0, 0.0), 2.0, quad_material));

    //for (int a = 0; a < 4; a++) {
    //    for (int b = 0; b < 1; b++) {
    //        point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

    //        if ((center - point3(4, 0.2, 0)).length() > 0.9) {
    //            shared_ptr<material> sphere_material;
    //            // glass
    //            sphere_material = make_shared<dielectric>(1.5);
    //            world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //            auto center2 = center + vec3(0, random_double(0, .5), 0);
    //            world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));

    //        }
    //    }
    //}

    world = hittable_list(make_shared<bvh_node>(world));

    // Set up the camera's public variables
    camera cam;

    cam.aspect_ratio = 9.0 / 16.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth = 110;
    cam.vfov = 74; // Smaller values are more zoomed in/cropped
    cam.background = color(0.70, 0.80, 1.00);

    // Position and direction
    cam.lookfrom = point3(-3, 1, 2);
    //cam.lookfrom = point3(2, 1, -1);

    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    // DoF
    cam.defocus_angle = 1.0; // Low values leads to less defocus blur
    cam.focus_dist = 4.4; // The focus distance should match the distance from the camera to the object of interest

    cam.render(world);
}

int main() {
    switch (1) {
        case 1: scene1(); break;
    }

}
