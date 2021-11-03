//
// Created by Aleksandr Lvov on 2021-11-02.
//

#include "HittableList.h"

std::optional<HitRecord> HittableList::intersect(const Ray &ray, value_t min_dist, value_t max_dist) const {
    auto result = std::optional<HitRecord>{};
    auto closest_distance = kInfinity;
    for (const auto& obj : objects) {
        auto hit = obj->intersect(ray, min_dist, max_dist);
        if (hit && hit->distance < closest_distance) {
            result = hit;
            closest_distance = hit->distance;
        }
    }

    return result;
}
