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
    FloatT radius_;

    std::shared_ptr<Material> material_;

public:
    Sphere(Point3 center, FloatT radius, std::shared_ptr<Material> material)
    : center_(center), radius_(radius), material_(std::move(material)) {}

    std::optional<HitRecord> intersect(
            const Ray& ray, FloatT min_dist, FloatT max_dist) const override;

    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};

std::pair<FloatT, FloatT> getSphereUV(const Vec3& p);
