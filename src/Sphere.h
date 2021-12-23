//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include <utility>

#include "Vec3.h"
#include "Ray.h"
#include "Hittable.h"

class Sphere final: public Hittable {
    Point3 center_;
    value_t radius_;

    std::shared_ptr<Material> material_;

public:
    Sphere(Point3 center, value_t radius, std::shared_ptr<Material> material)
    : center_(center), radius_(radius), material_(std::move(material)) {}

    std::optional<HitRecord> intersect(
            const Ray& ray, value_t min_dist, value_t max_dist) const override;

    std::optional<AABB> boundingBox(value_t time0, value_t time1) const override;
};

std::pair<value_t, value_t> getSphereUV(const Vec3& p);
