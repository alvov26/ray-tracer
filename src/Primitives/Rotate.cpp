//
// Created by Aleksandr Lvov on 2021-12-29.
//

#include "Rotate.h"

RotateY::RotateY(std::shared_ptr<Hittable> obj, FloatT angle) : obj_(std::move(obj)) {
    auto rad = degreesToRadians(angle);
    sin_theta_ = std::sin(rad);
    cos_theta_ = std::cos(rad);
    aabb_ = obj_->boundingBox(0, 1);

    Point3 min( kInfinity,  kInfinity,  kInfinity);
    Point3 max(-kInfinity, -kInfinity, -kInfinity);

    for (auto i : range(2)) {
        for (auto j : range(2)) {
            for (auto k : range(2)) {
                auto x = i* aabb_->max().x() + (1-i)* aabb_->min().x();
                auto y = j* aabb_->max().y() + (1-j)* aabb_->min().y();
                auto z = k* aabb_->max().z() + (1-k)* aabb_->min().z();

                auto newx =  cos_theta_*x + sin_theta_*z;
                auto newz = -sin_theta_*x + cos_theta_*z;

                auto tester = Vec3(newx, y, newz);

                for (auto c : range(3)) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }
    aabb_ = AABB(min, max);
}

std::optional<HitRecord> RotateY::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    auto origin = ray.origin();
    auto direction = ray.direction();

    origin[0] = cos_theta_ * ray.origin()[0] - sin_theta_ * ray.origin()[2];
    origin[2] = sin_theta_ * ray.origin()[0] + cos_theta_ * ray.origin()[2];

    direction[0] = cos_theta_ * ray.direction()[0] - sin_theta_ * ray.direction()[2];
    direction[2] = sin_theta_ * ray.direction()[0] + cos_theta_ * ray.direction()[2];

    auto rotated_ray = Ray(origin, direction, ray.time());

    auto hit = obj_->intersect(rotated_ray, min_dist, max_dist);
    if (!hit) return {};

    auto point = hit->point;
    auto normal = hit->normal;

    point[0] =  cos_theta_ * hit->point[0] + sin_theta_ * hit->point[2];
    point[2] = -sin_theta_ * hit->point[0] + cos_theta_ * hit->point[2];

    normal[0] =  cos_theta_ * hit->normal[0] + sin_theta_ * hit->normal[2];
    normal[2] = -sin_theta_ * hit->normal[0] + cos_theta_ * hit->normal[2];

    hit->point = point;
    hit->setFaceNormal(rotated_ray, normal);

    return hit;
}

std::optional<AABB> RotateY::boundingBox(FloatT /*time0*/, FloatT /*time1*/) const {
    return aabb_;
}
