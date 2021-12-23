//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "AABB.h"
#include "Ray.h"
#include "Vec3.h"
#include <optional>

class Material;

struct HitRecord {
    Point3 point;
    Vec3 normal;
    value_t distance;
    value_t u, v;
    bool front_face;
    std::shared_ptr<Material> material;

    void setFaceNormal(const Ray &r, const Vec3 &outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual std::optional<HitRecord> intersect(
            const Ray &ray, value_t min_dist, value_t max_dist) const = 0;

    virtual std::optional<HitRecord> intersect(const Ray &ray) const final {
        return intersect(ray, kEpsilon, kInfinity);
    }

    virtual std::optional<AABB> boundingBox(value_t time0, value_t time1) const = 0;
    ~Hittable() = default;
};
