//
// Created by Aleksandr Lvov on 2021-12-27.
//

#include "Rect.h"

std::optional<HitRecord> XYRect::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    auto dist = (k_ - ray.origin().z()) / ray.direction().z();
    if (dist < min_dist || dist > max_dist)
        return {};

    auto point = ray.at(dist);
    if (point.x() < x0_ || point.x() > x1_ || point.y() < y0_ || point.y() > y1_)
        return {};

    auto record = HitRecord();
    record.point = point;
    record.distance = dist;
    record.u = (point.x() - x0_)/(x1_ - x0_);
    record.v = (point.y() - y0_)/(y1_ - y0_);
    record.setFaceNormal(ray, Vec3(0, 0, 1));
    record.material = material_;

    return record;
}

XYRect::XYRect(FloatT x0, FloatT x1, FloatT y0, FloatT y1, FloatT k, std::shared_ptr<Material> material)
        : x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), material_(std::move(material)) {}

std::optional<AABB> XYRect::boundingBox(FloatT /*time0*/, FloatT /*time1*/) const {
    return AABB(Point3(x0_, y0_, k_ - kEpsilon), Point3(x1_, y1_, k_ + kEpsilon));
}

std::optional<HitRecord> XZRect::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    auto dist = (k_ - ray.origin().y()) / ray.direction().y();
    if (dist < min_dist || dist > max_dist)
        return {};

    auto point = ray.at(dist);
    if (point.x() < x0_ || point.x() > x1_ || point.z() < z0_ || point.z() > z1_)
        return {};

    auto record = HitRecord();
    record.point = point;
    record.distance = dist;
    record.u = (point.x() - x0_)/(x1_ - x0_);
    record.v = (point.z() - z0_)/(z1_ - z0_);
    record.setFaceNormal(ray, Vec3(0, 1, 0));
    record.material = material_;

    return record;
}

XZRect::XZRect(FloatT x0, FloatT x1, FloatT z0, FloatT z1, FloatT k, std::shared_ptr<Material> material)
        : x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), material_(std::move(material)) {}

std::optional<AABB> XZRect::boundingBox(FloatT /*time0*/, FloatT /*time1*/) const {
    return AABB(Point3(x0_, k_ - kEpsilon, z0_), Point3(x1_, k_ + kEpsilon, z1_));
}

std::optional<HitRecord> YZRect::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    auto dist = (k_ - ray.origin().x()) / ray.direction().x();
    if (dist < min_dist || dist > max_dist)
        return {};

    auto point = ray.at(dist);
    if (point.y() < y0_ || point.y() > y1_ || point.z() < z0_ || point.z() > z1_)
        return {};

    auto record = HitRecord();
    record.point = point;
    record.distance = dist;
    record.u = (point.y() - y0_)/(y1_ - y0_);
    record.v = (point.z() - z0_)/(z1_ - z0_);
    record.setFaceNormal(ray, Vec3(1, 0, 0));
    record.material = material_;

    return record;
}

YZRect::YZRect(FloatT y0, FloatT y1, FloatT z0, FloatT z1, FloatT k, std::shared_ptr<Material> material)
        : y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), material_(std::move(material)) {}

std::optional<AABB> YZRect::boundingBox(FloatT /*time0*/, FloatT /*time1*/) const {
    return AABB(Point3(k_ - kEpsilon, y0_, z0_), Point3(k_ + kEpsilon, y1_, z1_));
}
