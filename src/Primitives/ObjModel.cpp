//
// Created by Aleksandr Lvov on 2021-12-29.
//

#include "ObjModel.h"
#include "../../external/tiny_obj_loader.h"

#include <utility>
#include <iostream>
#include <array>

ObjModel::ObjModel(const std::string& filename, const std::shared_ptr<Material>& material, FloatT scale)
    : material_(material)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./";
    reader_config.triangulate = true;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader Error: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cerr << "TinyObjReader Warning: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    HittableList triangles;

    for (auto& shape : shapes) {
        auto& indices = shape.mesh.indices;
        for (auto it = std::begin(indices); it < std::end(indices);) {
            std::array<Point3, 3> vertices;
            std::array<Vec3, 3> normals;

            for (auto i : range(3)) {
                auto &idx = *(it);

                auto vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                auto vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                auto vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                vertices[i] = Point3(
                        static_cast<FloatT>(vx) * scale,
                        static_cast<FloatT>(vy) * scale,
                        static_cast<FloatT>(vz) * scale);

                if (idx.normal_index >= 0) {
                    auto nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    auto ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    auto nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

                    normals[i] = Vec3(nx, ny, nz);
                }
                ++it;
            }

            auto tri = std::make_shared<Triangle>(vertices[0], vertices[1], vertices[2], material);
            tri->setNormals(normals[0], normals[1], normals[2]);

            triangles.add(tri);
        }
    }

    data_ = BVH_Node(triangles, 0, 1);
}

std::optional<HitRecord> ObjModel::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    return data_.intersect(ray, min_dist, max_dist);
}

std::optional<AABB> ObjModel::boundingBox(FloatT time0, FloatT time1) const {
    return data_.boundingBox(time0, time1);
}

Triangle::Triangle(const Point3 &v0, const Point3 &v1, const Point3 &v2, std::shared_ptr<Material> material)
    : v0_(v0), v1_(v1), v2_(v2), material_(std::move(material)) {}

std::optional<HitRecord> Triangle::intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
    auto v0v1 = v1_ - v0_;
    auto v0v2 = v2_ - v0_;
    auto pvec = ray.direction.cross(v0v2);
    auto det = v0v1.dot(pvec);

    if (std::abs(det) < kEpsilon) return {};

    auto inv_det = 1 / det;

    auto tvec = ray.origin - v0_;
    auto u = tvec.dot(pvec) * inv_det;
    if (u < 0 || u > 1) return {};

    auto qvec = tvec.cross(v0v1);
    auto v = ray.direction.dot(qvec) * inv_det;
    if (v < 0 || u + v > 1) return {};

    auto t = v0v2.dot(qvec) * inv_det;
    if (t < min_dist || t > max_dist)
        return {};

    auto normal = n2_ * v + n1_ * u + n0_ * (1 - u - v);

    auto hit = HitRecord();
    hit.point = ray.at(t);
    hit.distance = t;
    hit.u = u;
    hit.v = v;
    hit.setFaceNormal(ray, normal);
    hit.material = material_;

    return hit;
}

std::optional<AABB> Triangle::boundingBox(FloatT /*time0*/, FloatT /*time1*/) const {
    auto min_x = std::min({v0_.x(), v1_.x(), v2_.x()});
    auto min_y = std::min({v0_.y(), v1_.y(), v2_.y()});
    auto min_z = std::min({v0_.z(), v1_.z(), v2_.z()});

    auto max_x = std::max({v0_.x(), v1_.x(), v2_.x()});
    auto max_y = std::max({v0_.y(), v1_.y(), v2_.y()});
    auto max_z = std::max({v0_.z(), v1_.z(), v2_.z()});

    return AABB(Vec3(min_x, min_y, min_z), Vec3(max_x, max_y, max_z));
}

void Triangle::setNormals(Vec3 n0, Vec3 n1, Vec3 n2) {
    n0_ = n0;
    n1_ = n1;
    n2_ = n2;
}
