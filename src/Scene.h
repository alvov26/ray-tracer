//
// Created by Aleksandr Lvov on 2022-03-05.
//

#pragma once

#include "Camera.h"
#include "Primitives/HittableList.h"

#include <future>

using ImageBuffer = std::vector<Colour>;

struct Scene {
    HittableList world;
    Colour backgroundColour = Colour(0, 0, 0);
    Camera::Builder cameraBuilder;
};

Colour castRay(const Ray&, const Hittable&, const Colour& background, int depth);
Colour gammaCorrection(Colour);
void renderMultiThread(int threadCount, const Scene&, Image&, int samplesPerPixel, int depth);
