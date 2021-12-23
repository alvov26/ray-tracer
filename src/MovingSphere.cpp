//
// Created by Aleksandr Lvov on 2021-11-05.
//

#include "MovingSphere.h"
#include "Sphere.h"

std::optional<HitRecord> MovingSphere::intersect(const Ray &ray, value_t min_dist, value_t max_dist) const {
    auto center = this->center(ray.time());
    auto oc = ray.origin() - center;

    auto a = ray.direction().length_squared();
    auto half_b = ray.direction().dot(oc);
    auto c = oc.length_squared() - radius_ * radius_;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return {};

    auto sqrt_d = std::sqrt(discriminant);
    auto root = (-half_b - sqrt_d) / a;// also distance
    if (root < min_dist || root > max_dist) {
        root = (-half_b + sqrt_d) / a;
        if (root < min_dist || root > max_dist)
            return {};
    }

    auto result = HitRecord();
    result.point = ray.at(root);
    result.distance = root;
    result.material = material_;

    auto outward_normal = (result.point - center) / radius_;
    result.setFaceNormal(ray, outward_normal);

    auto [u, v] = getSphereUV(outward_normal);
    result.u = u;
    result.v = v;

    return result;
}

std::optional<AABB> MovingSphere::boundingBox(value_t time0, value_t time1) const {
    auto box1 = AABB{
            center(time0) - Vec3(radius_, radius_, radius_),
            center(time0) + Vec3(radius_, radius_, radius_),
    };
    auto box2 = AABB{
            center(time1) - Vec3(radius_, radius_, radius_),
            center(time1) + Vec3(radius_, radius_, radius_),
    };
    return surroundingBox(box1, box2);
}
