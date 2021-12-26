#include "src/Image.h"
#include "src/Camera.h"
#include "src/HittableList.h"
#include "src/Sphere.h"
#include "src/Material.h"
#include "src/Timer.h"
#include "src/MovingSphere.h"

#include <iostream>
#include <sstream>
#include <future>

Colour castRay(const Ray& ray, const Hittable& hittable, const Colour& background, int depth) {
    if (depth == 0) return {0, 0, 0};

    auto hit = hittable.intersect(ray);
    if (!hit) return background;

    auto emitted = hit->material->emit(hit->u, hit->v, hit->point);
    auto scattered = hit->material->scatter(ray, *hit);
    if (!scattered) return emitted;

    return emitted + scattered->attenuation * castRay(scattered->ray, hittable, background, depth - 1);
}

Colour gammaCorrection(Colour c, FloatT gamma = 2){
    return { std::sqrt(c.x()), std::sqrt(c.y()), std::sqrt(c.z()) };
}

int main() {
    const auto width  = kDebug ? 640 : 1280;
    const auto height = kDebug ? 360 : 720;
    const auto aspect_ratio = FloatT(width) / FloatT(height);
    const auto samples_per_pixel = kDebug ? 100 : 3000;
    const auto max_depth = kDebug ? 50 : 100;

    auto image  = Image(width, height);

    auto world = HittableList();

    using std::make_shared;

    auto green_texture = make_shared<SolidColour>(Colour(0.3, 0.9, 0.3));
    auto blue_texture  = make_shared<SolidColour>(Colour(0.3, 0.5, 0.9));
    auto steel_texture = make_shared<SolidColour>(Colour(0.8, 0.8, 0.8));
    auto gold_texture  = make_shared<SolidColour>(Colour(0.8, 0.6, 0.2));

    auto b_ball_texture    = make_shared<ImageTexture>(Image::readFromFile("../16074.jpg"));
    auto world_map_texture = make_shared<ImageTexture>(Image::readFromFile("../earthmap.jpg"));
    auto checker_texture   = make_shared<CheckerTexture>(green_texture, blue_texture);

    using Lambertian = Lambertian<false>;
    auto blue    = make_shared<Lambertian>(blue_texture);
    auto green   = make_shared<Lambertian>(green_texture);
    auto globe   = make_shared<Lambertian>(world_map_texture);
    auto checker = make_shared<Lambertian>(checker_texture);

    auto steel   = make_shared<Metal>(steel_texture, 0.1);
    auto gold    = make_shared<Metal>(gold_texture, 0.7);
    auto b_ball  = make_shared<Metal>(b_ball_texture, 0.1);

    auto glass   = make_shared<Dielectric>(1.5);
    auto light   = make_shared<DiffuseLight>(Colour(48, 48, 48));


    world.add(make_shared<Sphere>(Point3( 0, 0,-1), 0.5, b_ball));
    world.add(make_shared<Sphere>(Point3(-1, 0,-1), 0.5, glass));
    world.add(make_shared<Sphere>(Point3( 1, 0,-1), 0.5, globe));
    world.add(make_shared<Sphere>(Point3( 2, 2, 0), 0.2, light));
    world.add(make_shared<Sphere>(Point3(-3, 2, 0), 0.2, light));
    world.add(make_shared<Sphere>(Point3( 0,-100.5,-1), 100, checker));
    world.add(make_shared<MovingSphere>(Point3( 2, 0, -1), 0.5, Vec3(0, 0.5, 0), steel));

    auto look_from = Point3(0.5, 1.5, 4);
    auto look_at   = Point3(0.5, 0,  -1);
    auto up_view   = Vec3(0, 1, 0);
    auto camera    = Camera(
            look_from,
            look_at,
            up_view,
            45,
            aspect_ratio,
            1/16.,
            (look_from - look_at).length(),
            0, 1);

    auto background = Colour(0.3, 0.3, 0.7);

    auto time = Timer();

    /*
    for (auto x : range(width)) {
        for (auto y : range(height)) {
            auto colour = Colour();
            for (auto s : range(samples_per_pixel)){
                auto h = (FloatT(x) + randomFloatT()) / (width-1);
                auto v = (FloatT(height - y) + randomFloatT()) / (height-1);
                auto ray = camera.getRay(h, v);
                colour = colour + castRay(ray, world, background, max_depth);
            }
            colour = gammaCorrection(colour / samples_per_pixel);
            image.at(x, y) = Pixel::fromVec3(colour);
        }
        std::clog << '\r' << x + 1 << '/' << width;
    }
    */
    {
        const auto page_count = kDebug ? 2 : 4;
        std::vector<std::future<void>> futures;
        auto page_size = width / page_count;
        for (auto page: range(page_count)) {
            futures.push_back(std::async([&](auto page_num) {
                auto local_timer = Timer();
                for (auto x: range(page_size * page_num, page_size * page_num + page_size)) {
                    for (auto y: range(height)) {
                        auto colour = Colour();
                        for (auto s: range(samples_per_pixel)) {
                            auto h = (FloatT(x) + randomFloatT()) / (width - 1);
                            auto v = (FloatT(height - y) + randomFloatT()) / (height - 1);
                            auto ray = camera.getRay(h, v);
                            colour = colour + castRay(ray, world, background, max_depth);
                        }
                        colour = gammaCorrection(colour / samples_per_pixel);
                        image.at(x, y) = Pixel::fromVec3(colour);
                    }
                    if (page_num == 0) {
                        std::stringstream status;
                        status << '\r' << (x + 1)  << '/' << page_size;
                        std::clog << status.str();
                    }
                }
                std::stringstream message;
                message << "\nThread " << page_num + 1 << " finished rendering in "
                << local_timer.seconds() << " seconds\n";
                std::clog << message.str();
            }, page));
        }
    }

    std::clog << "\nFinished in " << time.seconds() << " seconds";

    image.writeToFile("out.jpg");
    return 0;
}
