//
// Created by Aleksandr Lvov on 2021-12-29.
//

#include "Translate.h"

Translate::Translate(const std::shared_ptr<Hittable> &obj, const Vec3 &offset)
    : obj_(obj), offset_(offset) {}

std::optional<HitRecord> Translate::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    auto moved_ray = Ray{
            .origin = ray.origin - offset_,
            .direction = ray.direction,
            .time = ray.time
    };
    auto hit = obj_->intersect(moved_ray, min_dist, max_dist);
    if (!hit) return {};

    hit->point = hit->point + offset_;
    hit->setFaceNormal(moved_ray, hit->normal);
    return hit;
}

std::optional<AABB> Translate::boundingBox(FloatT time0, FloatT time1) const {
    auto box = obj_->boundingBox(time0, time1);
    if (!box) return {};
    return AABB(box->min() + offset_, box->max() + offset_);
}
