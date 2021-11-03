//
// Created by Aleksandr Lvov on 2021-11-02.
//

#include "Sphere.h"

std::optional<HitRecord> Sphere::intersect(const Ray &ray, value_t min_dist, value_t max_dist) const {
    auto oc = ray.origin() - center_;

    auto a = ray.direction().length_squared();
    auto half_b = ray.direction().dot(oc);
    auto c = oc.length_squared() - radius_ * radius_;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return {};

    auto sqrt_d = std::sqrt(discriminant);
    auto root = (-half_b - sqrt_d) / a; // also distance
    if (root < min_dist || root > max_dist) {
        root = (-half_b + sqrt_d) / a;
        if (root < min_dist || root > max_dist)
            return {};
    }

    auto result = HitRecord();
    result.point = ray.at(root);
    result.distance = root;
    result.material = material_;

    auto outward_normal = (result.point - center_) / radius_;
    result.setFaceNormal(ray, outward_normal);

    return result;
}