#include "src/Image.h"
#include "src/Camera.h"
#include "src/HittableList.h"
#include "src/Sphere.h"
#include "src/Material.h"

Colour castRay(const Ray& ray, const Hittable& hittable, int depth) {
    if (depth == 0) return Colour(0, 0, 0);
    auto hit = hittable.intersect(ray);
    if (hit) {
        auto scattered = hit->material->scatter(ray, *hit);
        if (scattered) {
            return scattered->attenuation * castRay(scattered->ray, hittable, depth - 1);
        }
        return Colour(0, 0, 0);
    }
    auto t = value_t((ray.direction().y() + 1) * 0.5);
    return Colour(1.0, 1.0, 1.0)*(1.f - t) + Colour(0.5, 0.7, 1.0)*t;
}

Colour gammaCorrection(Colour c, value_t gamma = 2){
    return { std::sqrt(c.x()), std::sqrt(c.y()), std::sqrt(c.z()) };
}

int main() {
    const auto width  = kDebug ? 640 : 1280;
    const auto height = kDebug ? 360 : 720;
    const auto aspect_ratio = value_t(width) / value_t(height);
    const auto samples_per_pixel = kDebug ? 10 : 1000;
    const auto max_depth = kDebug ? 50 : 100;

    auto image  = Image(width, height);

    auto world = HittableList();
    auto blue = std::make_shared<Lambertian<true>>(Colour(0.3, 0.5, 0.9));
    auto green = std::make_shared<Lambertian<true>>(Colour(0.3, 0.9, 0.3));
    auto steel = std::make_shared<Metal>(Colour(0.8, 0.8, 0.8), 0.1);
    auto gold = std::make_shared<Metal>(Colour(0.8, 0.6, 0.2), 0.7);
    auto glass = std::make_shared<Dielectric>(1.5);

    world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5, blue));
    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100, green));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, glass));
    world.add(std::make_shared<Sphere>(Point3( 1.0, 0.0, -1.0), 0.5, gold));

    auto look_from = Point3(-2, 2, 1);
    auto look_at   = Point3(0, 0, -1);
    auto camera = Camera(look_from, look_at, Vec3(0, 1, 0),
                         60, aspect_ratio, 0, (look_from - look_at).length());

    for (auto x : range(width)) {
        for (auto y : range(height)) {
            auto colour = Colour();
            for (auto s : range(samples_per_pixel)){
                auto h = (value_t(x) + random_value_t()) / (width-1);
                auto v = (value_t(height - y) + random_value_t()) / (height-1);
                auto ray = camera.getRay(h, v);
                colour = colour + castRay(ray, world, max_depth);
            }
            colour = gammaCorrection(colour / samples_per_pixel);
            image.at(x, y) = Pixel::fromVec3(colour);
        }
    }

    image.writeToFile("out.jpg");
    return 0;
}
