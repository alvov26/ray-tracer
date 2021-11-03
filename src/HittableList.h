//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include <vector>
#include <memory>
#include "Hittable.h"
#include "Ray.h"

class HittableList final: public Hittable{
    std::vector<std::shared_ptr<Hittable>> objects;

public:
    void add(std::shared_ptr<Hittable> obj) {
        objects.push_back(obj);
    }

    std::optional<HitRecord> intersect(
            const Ray &ray, value_t min_dist, value_t max_dist) const override;
};
