#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"
#include "helper.h"

class material; // This will be defined later


// Class representing a hit record with information about a ray-object intersection
class hit_record {
public:
    point3 p;                   // Point of intersection
    vec3 normal;                // Normal vector at the point of intersection
    shared_ptr<material> mat;   // Material pointer
    double t;                   // Parameter 't' representing the distance along the ray to the point of intersection
   
    double u; // Texture coordinates
    double v; // Texture coordinates

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

    virtual aabb bounding_box() const = 0;

};

class translate : public hittable {
public:

    translate(shared_ptr<hittable> p, const vec3& displacement)
        : object(p), offset(displacement)
    {
        bbox = object->bounding_box() + offset;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Move the ray backwards by the offset
        ray offset_r(r.origin() - offset, r.direction(), r.time());

        // Determine where (if any) an intersection occurs along the offset ray
        if (!object->hit(offset_r, ray_t, rec))
            return false;

        // Move the intersection point forwards by the offset
        rec.p += offset;

        return true;
    }

    aabb bounding_box() const override { return bbox; }


private:
    shared_ptr<hittable> object;
    vec3 offset;
    aabb bbox;

};

class rotate_x : public hittable {
public:
    rotate_x(shared_ptr<hittable> p, double angle) : object(p) {
        auto radians = degrees_to_radians(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        bbox = object->bounding_box();

        point3 min(infinity, infinity, infinity);
        point3 max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    auto newy = cos_theta * y - sin_theta * z;
                    auto newz = sin_theta * y + cos_theta * z;

                    vec3 tester(x, newy, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = aabb(min, max);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // figure out if we hit rotated object
        // if NOT hit
        //   return false
        // if hit
        //   Determine where the intersection is
        //   return true

        // Origin and direction start the function in "world space".
        auto origin = r.origin();
        auto direction = r.direction();

        // Change the ray from world space to object space (remember: world->object is the inverse of object->world)
        origin[1] = cos_theta * r.origin()[1] + sin_theta * r.origin()[2];
        origin[2] = -sin_theta * r.origin()[1] + cos_theta * r.origin()[2];

        direction[1] = cos_theta * r.direction()[1] + sin_theta * r.direction()[2];
        direction[2] = -sin_theta * r.direction()[1] + cos_theta * r.direction()[2];

        // In order for `ray rotated_r(origin, direction, r.time());` to identify true intersections, the ray rotated_r needs to be in "object space"
        ray rotated_r(origin, direction, r.time());

        // Determine where (if any) an intersection occurs in object space
        if (!object->hit(rotated_r, ray_t, rec))
            return false;

        // Change the intersection point from object space to world space
        auto p = rec.p;
        p[1] = cos_theta * rec.p[1] - sin_theta * rec.p[2];
        p[2] = sin_theta * rec.p[1] + cos_theta * rec.p[2];

        // Change the normal from object space to world space
        auto normal = rec.normal;
        normal[1] = cos_theta * rec.normal[1] - sin_theta * rec.normal[2];
        normal[2] = sin_theta * rec.normal[1] + cos_theta * rec.normal[2];

        rec.p = p;
        rec.normal = normal;

        return true;
    }
    aabb bounding_box() const override { return bbox; }

private:
    shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};


class rotate_y : public hittable {
public:
    rotate_y(shared_ptr<hittable> p, double angle) : object(p) {
        auto radians = degrees_to_radians(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        bbox = object->bounding_box();

        point3 min(infinity, infinity, infinity);
        point3 max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    auto newx = cos_theta * x + sin_theta * z;
                    auto newz = -sin_theta * x + cos_theta * z;

                    vec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = aabb(min, max);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // figure out if we hit rotated object
        // if NOT hit
        //   return false
        // if hit
        //   Determine where the intersection is
        //   return true

        // Origin and direction start the function in "world space".
        auto origin = r.origin(); 
        auto direction = r.direction();

        // Change the ray from world space to object space (remember: world->object is the inverse of object->world)
        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

        // In order for `ray rotated_r(origin, direction, r.time());` to identify true intersections, the ray rotated_r needs to be in "object space"
        ray rotated_r(origin, direction, r.time());

        // Determine where (if any) an intersection occurs in object space
        if (!object->hit(rotated_r, ray_t, rec))
            return false;

        // Change the intersection point from object space to world space
        auto p = rec.p;
        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

        // Change the normal from object space to world space
        auto normal = rec.normal;
        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

        rec.p = p;
        rec.normal = normal;

        return true;
    }
    aabb bounding_box() const override { return bbox; }

private:
    shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    aabb bbox;

};

class rotate_z : public hittable {
public:
    rotate_z(shared_ptr<hittable> p, double angle) : object(p) {
        auto radians = degrees_to_radians(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        bbox = object->bounding_box();

        point3 min(infinity, infinity, infinity);
        point3 max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    auto newx = cos_theta * x - sin_theta * y;
                    auto newy = sin_theta * x + cos_theta * y;

                    vec3 tester(newx, newy, z);

                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = aabb(min, max);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // figure out if we hit rotated object
        // if NOT hit
        //   return false
        // if hit
        //   Determine where the intersection is
        //   return true

        // Origin and direction start the function in "world space".
        auto origin = r.origin();
        auto direction = r.direction();

        // Change the ray from world space to object space (remember: world->object is the inverse of object->world)
        origin[0] = cos_theta * r.origin()[0] + sin_theta * r.origin()[1];
        origin[1] = -sin_theta * r.origin()[0] + cos_theta * r.origin()[1];

        direction[0] = cos_theta * r.direction()[0] + sin_theta * r.direction()[1];
        direction[1] = -sin_theta * r.direction()[0] + cos_theta * r.direction()[1];

        // In order for `ray rotated_r(origin, direction, r.time());` to identify true intersections, the ray rotated_r needs to be in "object space"
        ray rotated_r(origin, direction, r.time());

        // Determine where (if any) an intersection occurs in object space
        if (!object->hit(rotated_r, ray_t, rec))
            return false;

        // Change the intersection point from object space to world space
        auto p = rec.p;
        p[0] = cos_theta * rec.p[0] - sin_theta * rec.p[1];
        p[1] = sin_theta * rec.p[0] + cos_theta * rec.p[1];

        // Change the normal from object space to world space
        auto normal = rec.normal;
        normal[0] = cos_theta * rec.normal[0] - sin_theta * rec.normal[1];
        normal[1] = sin_theta * rec.normal[0] + cos_theta * rec.normal[1];

        rec.p = p;
        rec.normal = normal;

        return true;
    }
    aabb bounding_box() const override { return bbox; }

private:
    shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};



#endif
