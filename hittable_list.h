#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "aabb.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

// A hittable_list class that represents a list of hittable objects.
class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    // Constructors
    hittable_list() {}  // Default constructor
    hittable_list(shared_ptr<hittable> object) { add(object); }  // Constructor with initial object

    // Clear the list of hittable objects.
    void clear() { objects.clear(); }

    // Add a hittable object to the list.
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    // Check if the ray intersects with any hittable object in the list.
    // Updates the hit_record if an intersection is found.
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        // Iterate through each object in the list.
        for (const auto& object : objects) {
            // Check for intersection with the current object.
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;  // Update the hit record with the closest intersection.
            }
        }

        return hit_anything;
    }
    aabb bounding_box() const override { return bbox; }

private:
    aabb bbox;

};

#endif