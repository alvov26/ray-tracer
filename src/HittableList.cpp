//
// Created by Aleksandr Lvov on 2021-11-02.
//

#include "HittableList.h"

std::optional<HitRecord> HittableList::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    auto box = boundingBox(0, 1);
    if (!box->doesIntersect(ray, min_dist, max_dist)) return {};

    auto result = std::optional<HitRecord>{};
    auto closest_distance = kInfinity;
    for (const auto &obj : objects) {
        auto hit = obj->intersect(ray, min_dist, max_dist);
        if (hit && hit->distance < closest_distance) {
            result = hit;
            closest_distance = hit->distance;
        }
    }

    return result;
}

std::optional<AABB> HittableList::boundingBox(FloatT time0, FloatT time1) const {
    if (objects.empty()) return {};
    auto box = std::optional<AABB>{};
    for (const auto &obj : objects) {
        auto new_box = obj->boundingBox(time0, time1);
        if (!new_box) return {};
        if (!box) box = new_box;
        else
            box = surroundingBox(*box, *new_box);
    }
    return box;
}

const std::vector<std::shared_ptr<Hittable>> &HittableList::getObjects() const {
    return objects;
}
