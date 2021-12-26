//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "Hittable.h"
#include "Ray.h"
#include <memory>
#include <vector>

class HittableList final : public Hittable {
    std::vector<std::shared_ptr<Hittable>> objects;

public:
    const std::vector<std::shared_ptr<Hittable>> &getObjects() const;

public:
    void add(const std::shared_ptr<Hittable> &obj) {
        objects.push_back(obj);
    }

    std::optional<HitRecord> intersect(
            const Ray &ray, FloatT min_dist, FloatT max_dist) const override;

    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
