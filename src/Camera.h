//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "Image.h"
#include "Ray.h"
#include "Vec3.h"
#include <optional>

inline Vec3 randomVecInUnitDisk() {
    while (true) {
        auto p = Vec3::random();
        p.z() = 0;
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}

class Camera {
    Point3 origin_;
    Point3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;

    Vec3 w_, u_, v_;
    FloatT lens_radius_;
    FloatT start_time_, end_time_;

    Camera(
            Point3 look_from,
            Point3 look_at,
            Vec3 up_vec,
            FloatT fov,
            FloatT aspect_ratio,
            FloatT aperture,
            FloatT focus_dist,
            FloatT start_time = 0,
            FloatT end_time = 0) : start_time_(start_time), end_time_(end_time) {
        const auto theta = degreesToRadians(fov);
        const auto h = tan(theta / 2);

        const auto viewport_height = 2.0 * h;
        const auto viewport_width = aspect_ratio * viewport_height;

        w_ = (look_from - look_at).normalized();
        u_ = up_vec.cross(w_);
        v_ = w_.cross(u_);

        origin_ = look_from;
        horizontal_ = u_ * viewport_width * focus_dist;
        vertical_ = v_ * viewport_height * focus_dist;
        lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - w_ * focus_dist;

        lens_radius_ = aperture / 2;
    }
public:
    struct Builder {
        std::optional<Point3> lookFrom;
        std::optional<Point3> lookAt;
        Vec3 upVec = Vec3(0, 1, 0);
        FloatT fov = 45, aspectRatio = 1, aperture = 1/16.;
        std::optional<FloatT> focusDist;

        [[nodiscard]]
        Camera build() const try {
            return Camera{
                    lookFrom.value(), lookAt.value(),
                    upVec, fov, aspectRatio, aperture,
                    focusDist.value_or((lookFrom.value() - lookAt.value()).length())
            };
        } catch (std::bad_optional_access&) {
            throw std::runtime_error("Camera::Builder: not all fields were filled");
        }

        Camera buildForImage(const Image& img) const {
            auto builder = Builder(*this);
            builder.aspectRatio = FloatT(img.width()) / FloatT(img.height());
            return builder.build();
        }
    };

    Ray getRay(FloatT s, FloatT t) const {
        const auto rd = randomVecInUnitDisk() * lens_radius_;
        const auto offset = (u_ * rd.x()) + (v_ * rd.y());
        const auto direction = (lower_left_corner_ + horizontal_ * s +
                                vertical_ * t - origin_ - offset).normalized();
        return {
                .origin = origin_ + offset,
                .direction = direction,
                .time = randomFloatT(start_time_, end_time_)
        };
    }
};
