//
// Created by Aleksandr Lvov on 2022-01-06.
//

#include <iostream>
#include "ConstantMedium.h"
#include "Material.h"

std::optional<HitRecord> ConstantMedium::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    const bool enableDebug = false;
    const bool debugging = enableDebug && randomFloatT() < 0.00001;

    auto rec1 = boundary_->intersect(ray, -kInfinity, kInfinity);
    if (!rec1) return {};

    auto rec2 = boundary_->intersect(ray, rec1->distance+0.0001, kInfinity);
    if (!rec2) return {};

    if (debugging) std::cerr << "\nt_min=" << rec1->distance << ", t_max=" << rec2->distance << '\n';

    if (rec1->distance < min_dist) rec1->distance = min_dist;
    if (rec2->distance > max_dist) rec2->distance = max_dist;

    if (rec1->distance >= rec2->distance)
        return {};

    if (rec1->distance < 0)
        rec1->distance = 0;

    const auto ray_length = ray.direction().length();
    const auto distance_inside_boundary = (rec2->distance - rec1->distance) * ray_length;
    const auto hit_distance = neg_inv_density_ * std::log(randomFloatT());

    if (hit_distance > distance_inside_boundary)
        return {};

    auto rec = HitRecord();
    rec.distance = rec1->distance + hit_distance / ray_length;
    rec.point = ray.at(rec.distance);

    if (debugging) {
        std::cerr << "hit_distance = " <<  hit_distance << '\n'
                  << "rec.t = " <<  rec.distance << '\n'
                  << "rec.p = {" <<  rec.point.x() << ", " << rec.point.y() << ", " << rec.point.z() << "}\n";
    }

    rec.normal = Vec3::randomInSphere();  // arbitrary
    rec.front_face = true;     // also arbitrary
    rec.material = phase_function_;

    return rec;
}

std::optional<AABB> ConstantMedium::boundingBox(FloatT time0, FloatT time1) const {
    return boundary_->boundingBox(time0, time1);
}

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> b, FloatT d, const std::shared_ptr<Texture> &a)
    : boundary_(std::move(b)),
      phase_function_(std::make_shared<Isotropic>(a)),
      neg_inv_density_(-1/d)
{}
