//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "Vec3.h"
#include "Ray.h"

Vec3 randomVecInUnitDisk() {
    while (true) {
        auto p = Vec3::random();
        p.z() = 0;
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

class Camera {
    Point3 origin_;
    Point3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;

    Vec3 w, u, v;
    value_t lens_radius;

public:
    Camera(
            Point3 look_from,
            Point3 look_at,
            Vec3 up_vec,
            value_t fov,
            value_t aspect_ratio,
            value_t aperture,
            value_t focus_dist
            ) {
        auto theta = degreesToRadians(fov);
        auto h = tan(theta/2);

        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        w = (look_from - look_at).normalized();
        u = up_vec.cross(w);
        v = w.cross(u);

        origin_     = look_from;
        horizontal_ = u * viewport_width * focus_dist;
        vertical_   = v * viewport_height * focus_dist;
        lower_left_corner_ = origin_ - horizontal_/2 - vertical_/2 - w*  focus_dist;

        lens_radius = aperture / 2;
    }

    Ray getRay(value_t s, value_t t) const {
        auto rd = randomVecInUnitDisk() * lens_radius;
        auto offset = (u * rd.x()) + (v * rd.y());
        return {
            origin_ + offset,
            (lower_left_corner_ + horizontal_ * s + vertical_ * t - origin_ - offset).normalized()
        };
    }
};
