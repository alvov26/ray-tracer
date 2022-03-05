//
// Created by Aleksandr Lvov on 2021-12-27.
//

#pragma once

#include "Hittable.h"
#include <memory>
#include <optional>

class XYRect : public Hittable {
    FloatT x0_, x1_, y0_, y1_, k_;
    std::shared_ptr<Material> material_;

public:
    XYRect(FloatT x0, FloatT x1, FloatT y0, FloatT y1, FloatT k, std::shared_ptr<Material> material);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};

class XZRect : public Hittable {
    FloatT x0_, x1_, z0_, z1_, k_;
    std::shared_ptr<Material> material_;

public:
    XZRect(FloatT x0, FloatT x1, FloatT z0, FloatT z1, FloatT k, std::shared_ptr<Material> material);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};

class YZRect : public Hittable {
    FloatT y0_, y1_, z0_, z1_, k_;
    std::shared_ptr<Material> material_;

public:
    YZRect(FloatT y0, FloatT y1, FloatT z0, FloatT z1, FloatT k, std::shared_ptr<Material> material);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
