//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Texture.h"
#include "Vec3.h"
#include <optional>
#include <utility>

struct ScatterRecord {
    Ray ray;
    Colour attenuation;
};

class Material {
public:
    virtual std::optional<ScatterRecord> scatter(
            const Ray &ray, const HitRecord &hit) const = 0;
    virtual Colour emit(FloatT u, FloatT v, const Point3 &p) const {
        return {0, 0, 0};
    }
    virtual ~Material() = default;
};

template<bool UseHemisphere = true>
class Lambertian final : public Material {
    std::shared_ptr<Texture> albedo_;

public:
    explicit Lambertian(std::shared_ptr<Texture> a) : albedo_(std::move(a)) {}

    std::optional<ScatterRecord> scatter(const Ray &ray, const HitRecord &hit) const override {
        Vec3 scatter_direction;
        if constexpr (UseHemisphere) {
            scatter_direction = Vec3::randomInHemisphere(hit.normal);
        } else {
            scatter_direction = hit.normal + Vec3::randomInSphere();
            if (scatter_direction.isNearZero()) scatter_direction = hit.normal;
            else
                scatter_direction = scatter_direction.normalized();
        }
        return ScatterRecord{
                Ray(hit.point, scatter_direction, ray.time()),
                albedo_->value(hit.u, hit.v, hit.point),
        };
    }
};

Vec3 reflect(const Vec3 &vec, const Vec3 &normal) {
    return vec - normal * 2 * vec.dot(normal);
}

class Metal final : public Material {
    std::shared_ptr<Texture> albedo_;
    FloatT fuzz_;

public:
    Metal(std::shared_ptr<Texture> a, FloatT fuzz) : albedo_(std::move(a)), fuzz_(fuzz < 1 ? fuzz : 1) {}

    std::optional<ScatterRecord> scatter(const Ray &ray, const HitRecord &hit) const override {
        auto direction = reflect(ray.direction(), hit.normal).normalized();
        direction = direction + Vec3::randomInSphere() * fuzz_;
        if (direction.dot(hit.normal) > 0) return ScatterRecord{
                Ray(hit.point, direction, ray.time()),
                albedo_->value(hit.u, hit.v, hit.point)};
        return {};
    }
};

Vec3 refract(const Vec3 &vec, const Vec3 &normal, FloatT refractive_ratio) {
    auto cos_theta = std::min<FloatT>(normal.dot(-vec), 1.0);
    auto r_perp = (vec + normal * cos_theta) * refractive_ratio;
    auto r_para = normal * -std::sqrt(std::abs(1.0 - r_perp.length_squared()));
    return r_perp + r_para;
}

class Dielectric final : public Material {
    FloatT refractive_index_;

public:
    explicit Dielectric(FloatT refractive_index)
        : refractive_index_(refractive_index) {}

    std::optional<ScatterRecord> scatter(const Ray &ray, const HitRecord &hit) const override {
        FloatT refraction_ratio = hit.front_face ? 1 / refractive_index_ : refractive_index_;

        auto cos_theta = std::min<FloatT>(hit.normal.dot(-ray.direction()), 1.0);
        auto sin_theta = std::sqrt(1 - cos_theta * cos_theta);

        auto cannot_refract = sin_theta * refraction_ratio > 1;
        Vec3 scattered;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomFloatT()) {// only reflect
            scattered = reflect(ray.direction(), hit.normal);
        } else {// can refract
            scattered = refract(ray.direction(), hit.normal, refraction_ratio);
        }

        return ScatterRecord{
                Ray(hit.point, scattered.normalized(), ray.time()),
                Colour(1, 1, 1)};
    }

private:
    static double reflectance(FloatT cosine, FloatT ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class DiffuseLight : public Material {
    std::shared_ptr<Texture> emit_;

public:
    explicit DiffuseLight(const Colour &c) : emit_(std::make_shared<SolidColour>(c)) {}
    explicit DiffuseLight(std::shared_ptr<Texture> emit) : emit_(std::move(emit)) {}

    std::optional<ScatterRecord> scatter(const Ray &ray, const HitRecord &hit) const override {
        return {};
    }

    Colour emit(FloatT u, FloatT v, const Point3 &p) const override {
        return emit_->value(u, v, p);
    }
};
