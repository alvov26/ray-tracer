//
// Created by Aleksandr Lvov on 2021-12-29.
//

#pragma once

#include "BVH_Node.h"
#include "Hittable.h"

class ObjModel : public Hittable {
    BVH_Node data_;
    std::shared_ptr<Material> material_;

public:
    ObjModel(const std::string& filename, const std::shared_ptr<Material>& material, FloatT scale = 1);
    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};

class Triangle : public Hittable {
    Point3 v0_, v1_, v2_;
    Vec3 n0_, n1_, n2_;
    std::shared_ptr<Material> material_;

public:
    Triangle(const Point3 &v0, const Point3 &v1, const Point3 &v2, std::shared_ptr<Material> material);
    void setNormals(Vec3 n0, Vec3 n1, Vec3 n2);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
