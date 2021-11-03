//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include <optional>
#include "Vec3.h"
#include "Ray.h"

class Material;

struct HitRecord {
    Point3 point;
    Vec3 normal;
    value_t distance;
    bool front_face;
    std::shared_ptr<Material> material;

    void setFaceNormal(const Ray& r, const Vec3& outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class Hittable {
public:
    virtual std::optional<HitRecord> intersect(
            const Ray& ray, value_t min_dist, value_t max_dist) const = 0;

    virtual std::optional<HitRecord> intersect(const Ray& ray) const final {
        return intersect(ray, kEpsilon, kInfinity);
    }
    ~Hittable() = default;
};
