//
// Created by Aleksandr Lvov on 2022-03-06.
//

#include "Scene.h"
#include "Timer.h"
#include "Material.h"
#include <iostream>


Colour castRay(const Ray &ray, const Hittable &hittable, const Colour &background, int depth) {
    if (depth == 0) return {0, 0, 0};

    auto hit = hittable.intersect(ray);
    if (!hit) return background;

    auto emitted = hit->material->emit(hit->u, hit->v, hit->point);
    auto scattered = hit->material->scatter(ray, *hit);
    if (!scattered) return emitted;

    return emitted + scattered->attenuation * castRay(scattered->ray, hittable, background, depth - 1);
}

Colour gammaCorrection(Colour c) {
    return { std::sqrt(c.x()), std::sqrt(c.y()), std::sqrt(c.z()) };
}

ImageBuffer renderNSamples(const Scene &scene, const Image &image, int mySamplesPerPixel, int allSamplesPerPixel, int depth) {
    const auto width = image.width();
    const auto height = image.height();
    const auto camera = scene.cameraBuilder.buildForImage(image);

    ImageBuffer buffer(width * height);

    auto timer = Timer();
    for (auto x : range(width)) {
        for (auto y : range(height)) {
            auto colour = Colour();
            for (auto s : range(mySamplesPerPixel)){
                auto h = (FloatT(x) + randomFloatT()) / FloatT(width-1);
                auto v = (FloatT(height - y) + randomFloatT()) / FloatT(height-1);
                auto ray = camera.getRay(h, v);
                colour = colour + castRay(ray, scene.world, scene.backgroundColour, depth);
            }
            colour = gammaCorrection(colour / allSamplesPerPixel);
            buffer[x + y * width] = colour;
        }
    }

    std::clog << "Thread finished in " << timer.elapsed() << " seconds\n" << std::flush;

    return buffer;
}

void renderMultiThread(int threadCount, const Scene &scene, Image &image, int samplesPerPixel, int depth) {
    std::vector<std::future<ImageBuffer>> futures;
    for (auto i : range(threadCount - 1)) {
        futures.push_back(std::async([&] {
          return renderNSamples(scene, image, samplesPerPixel / threadCount, samplesPerPixel, depth);
        }));
    }
    auto my_buf = renderNSamples(scene, image, samplesPerPixel / threadCount, samplesPerPixel, depth);

    for (auto& f : futures) {
        auto other_buf = f.get();
        for (auto i : range(my_buf.size())) {
            my_buf[i] = my_buf[i] + other_buf[i] ;
        }
    }

    for (auto x : range(image.width())) {
        for (auto y : range(image.height())) {
            auto colour = my_buf[x + y * image.width()];
            image.at(x, y) = Pixel::fromColour(colour);
        }
    }
}

