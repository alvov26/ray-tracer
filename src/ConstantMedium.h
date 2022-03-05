//
// Created by Aleksandr Lvov on 2022-01-06.
//

#pragma once
#include <utility>

#include "Primitives/Hittable.h"
#include "Texture.h"

class ConstantMedium : public Hittable {
    std::shared_ptr<Hittable> boundary_;
    std::shared_ptr<Material> phase_function_;
    FloatT neg_inv_density_;

public:
    ConstantMedium(std::shared_ptr<Hittable> b, FloatT d, const std::shared_ptr<Texture>& a);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
