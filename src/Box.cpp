//
// Created by Aleksandr Lvov on 2021-12-29.
//

#include "Box.h"
#include "Rect.h"

Box::Box(const Point3 &p0, const Point3 &p1, const std::shared_ptr<Material> &material)
    : min_point_(p0), max_point_(p1), material_(material)
{
    using std::make_shared;

    sides_.add(make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
    sides_.add(make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material));

    sides_.add(make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
    sides_.add(make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material));

    sides_.add(make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
    sides_.add(make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material));
}

std::optional<HitRecord> Box::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    return sides_.intersect(ray, min_dist, max_dist);
}

std::optional<AABB> Box::boundingBox(FloatT time0, FloatT time1) const {
    return AABB(min_point_, max_point_);
}
