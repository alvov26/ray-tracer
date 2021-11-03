//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include <optional>
#include "Vec3.h"
#include "Ray.h"
#include "Hittable.h"

struct ScatterRecord {
    Ray ray;
    Colour attenuation;
};

class Material {
public:
    virtual std::optional<ScatterRecord> scatter(
            const Ray& ray, const HitRecord& hit) const = 0;
    ~Material() = default;
};

template<bool UseHemisphere = true>
class Lambertian final: public Material {
    Colour albedo_;
public:
    Lambertian(const Colour& a) : albedo_(a) {}

    std::optional<ScatterRecord> scatter(const Ray &ray, const HitRecord &hit) const override {
        Vec3 scatter_direction;
        if constexpr(UseHemisphere) {
            scatter_direction = Vec3::randomInHemisphere(hit.normal);
        } else {
            scatter_direction = hit.normal + Vec3::random().normalized();
            if (scatter_direction.isNearZero()) scatter_direction = hit.normal;
            else scatter_direction = scatter_direction.normalized();
        }
        return ScatterRecord{
                Ray(hit.point, scatter_direction),
                albedo_
        };
    }
};

Vec3 reflect(const Vec3& vec, const Vec3& normal) {
    return vec - normal * 2 * vec.dot(normal);
}

class Metal final: public Material {
    Colour albedo_;
    value_t fuzz_;
public:
    Metal(const Colour& a, value_t fuzz) : albedo_(a), fuzz_(fuzz < 1 ? fuzz : 1) {}

    std::optional<ScatterRecord> scatter(const Ray &ray, const HitRecord &hit) const override {
        auto direction = reflect(ray.direction(), hit.normal).normalized();
        direction = direction + Vec3::random().normalized() * fuzz_;
        if (direction.dot(hit.normal) > 0) return ScatterRecord{
            Ray(hit.point, direction),
            albedo_
        };
        return {};
    }
};

Vec3 refract(const Vec3& vec, const Vec3& normal, value_t refractive_ratio) {
    auto cos_theta = std::min(normal.dot(-vec), 1.0);
    auto r_perp = (vec + normal * cos_theta) * refractive_ratio;
    auto r_para = normal * -std::sqrt(std::abs(1.0 - r_perp.length_squared()));
    return r_perp + r_para;
}

class Dielectric final: public Material {
    Colour albedo_;
    value_t refractive_index_;
public:
    Dielectric(value_t refractive_index, const Colour& a = Colour(1, 1, 1))
    : refractive_index_(refractive_index), albedo_(a) {}

    std::optional<ScatterRecord> scatter(const Ray &ray, const HitRecord &hit) const override {
        value_t refraction_ratio = hit.front_face ? 1 / refractive_index_ : refractive_index_;

        auto cos_theta = std::min(hit.normal.dot(-ray.direction()), 1.0);
        auto sin_theta = std::sqrt(1 - cos_theta*cos_theta);

        auto cannot_refract = sin_theta * refraction_ratio > 1;
        Vec3 scattered;
        if (cannot_refract
        || reflectance(cos_theta, refraction_ratio) > random_value_t()
        ) { // only reflect
            scattered = reflect(ray.direction(), hit.normal);
        } else { // can refract
            scattered = refract(ray.direction(), hit.normal, refraction_ratio);
        }

        return ScatterRecord{
            Ray(hit.point, scattered.normalized()),
            albedo_
        };
    }

private:
    static double reflectance(value_t cosine, value_t ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)* std::pow((1 - cosine) ,5);
    }
};
