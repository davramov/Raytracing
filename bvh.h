#ifndef BVH_H
#define BVH_H

#include "helper.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

// Bounding Volume Hierarchy
class bvh_node : public hittable {
public:
    // Constructor for building a BVH from a hittable_list
    bvh_node(const hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}

    // Constructor for building a BVH from a vector of hittable objects within a given range
    bvh_node(const std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
        auto objects = src_objects; // Create a modifiable array of the source scene objects

        // Randomly choose an axis for splitting
        int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_compare
            : (axis == 1) ? box_y_compare
            : box_z_compare;

        size_t object_span = end - start;

        // If there is only one object, set both left and right pointers to it
        if (object_span == 1) {
            left = right = objects[start];
        }
        // If there are two objects, compare them based on the chosen axis and assign accordingly
        else if (object_span == 2) {
            if (comparator(objects[start], objects[start + 1])) {
                left = objects[start];
                right = objects[start + 1];
            }
            else {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        // If there are more than two objects, sort them along the chosen axis and recursively build the BVH
        else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }

        // Compute the bounding box for the current node
        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

    // Check if the ray hits the BVH node
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (!bbox.hit(r, ray_t))
            return false;

        // Check for intersections with the left and right subtrees
        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        // Return true if the ray hits either the left or right subtree
        return hit_left || hit_right;
    }

    // Get the bounding box of the BVH node
    aabb bounding_box() const override { return bbox; }

private:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb bbox;

    // Static helper function for comparing bounding boxes along a specified axis
    static bool box_compare(
        const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index
    ) {
        return a->bounding_box().axis(axis_index).min < b->bounding_box().axis(axis_index).min;
    }

    // Static helper function for comparing bounding boxes along the x-axis
    static bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 0);
    }

    // Static helper function for comparing bounding boxes along the y-axis
    static bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 1);
    }

    // Static helper function for comparing bounding boxes along the z-axis
    static bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 2);
    }

};

#endif
