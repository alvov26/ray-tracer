//
// Created by Aleksandr Lvov on 2021-11-02.
//

#include "Sphere.h"

std::optional<HitRecord> Sphere::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
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

    auto [u, v] = getSphereUV(outward_normal);
    result.u = u;
    result.v = v;

    return result;
}

std::optional<AABB> Sphere::boundingBox(FloatT time0, FloatT time1) const {
    return AABB{
        center_ - Vec3(radius_, radius_, radius_),
        center_ + Vec3(radius_, radius_, radius_)
    };
}

std::pair<FloatT, FloatT> getSphereUV(const Vec3 &n) {
    auto theta = std::acos(-n.x());
    auto phi = atan2(-n.z(), n.x()) + kPi;
    auto u = phi / (2*kPi);
    auto v = asin(n.y()) / kPi + 0.5;
    return {u, v};
}
