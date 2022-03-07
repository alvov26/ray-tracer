//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "../AABB.h"
#include "../Ray.h"
#include "../Vec3.h"
#include <optional>

class Material;

struct HitRecord {
    Point3 point;
    Vec3 normal;
    FloatT distance;
    FloatT u, v;
    bool front_face;
    std::shared_ptr<Material> material;

    void setFaceNormal(const Ray &r, const Vec3 &outward_normal) {
        front_face = r.direction.dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual std::optional<HitRecord> intersect(
            const Ray &ray, FloatT min_dist, FloatT max_dist) const = 0;

    virtual std::optional<HitRecord> intersect(const Ray &ray) const final {
        return intersect(ray, kEpsilon, kInfinity);
    }

    virtual std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const = 0;
    virtual ~Hittable() = default;
};
