#include "src/Camera.h"
#include "src/ConstantMedium.h"
#include "src/Image.h"
#include "src/Material.h"
#include "src/Primitives/Box.h"
#include "src/Primitives/HittableList.h"
#include "src/Primitives/MovingSphere.h"
#include "src/Primitives/ObjModel.h"
#include "src/Primitives/Rect.h"
#include "src/Primitives/Rotate.h"
#include "src/Primitives/Sphere.h"
#include "src/Primitives/Translate.h"
#include "src/Scene.h"
#include "src/Timer.h"

#include <future>
#include <iostream>
#include <sstream>

HittableList fourBallScene(){
    using std::make_shared;

    auto world = HittableList();

    auto green_texture = make_shared<SolidColour>(Colour(0.3, 0.9, 0.3));
    auto blue_texture  = make_shared<SolidColour>(Colour(0.3, 0.5, 0.9));
    auto steel_texture = make_shared<SolidColour>(Colour(0.8, 0.8, 0.8));
    auto gold_texture  = make_shared<SolidColour>(Colour(0.8, 0.6, 0.2));
    auto red_texture   = make_shared<SolidColour>(Colour(0.8, 0.2, 0.2));

    auto b_ball_texture    = make_shared<ImageTexture>(Image::readFromFile("../16074.jpg"));
    auto world_map_texture = make_shared<ImageTexture>(Image::readFromFile("../earthmap.jpg"));
    auto checker_texture   = make_shared<CheckerTexture>(green_texture, blue_texture);

    using Lambertian = Lambertian<false>;
    auto blue    = make_shared<Lambertian>(blue_texture);
    auto green   = make_shared<Lambertian>(green_texture);
    auto globe   = make_shared<Lambertian>(world_map_texture);
    auto checker = make_shared<Lambertian>(checker_texture);
    auto red     = make_shared<Lambertian>(red_texture);

    auto steel   = make_shared<Metal>(steel_texture, 0.1);
    auto gold    = make_shared<Metal>(gold_texture, 0.7);
    auto b_ball  = make_shared<Metal>(b_ball_texture, 0.1);

    auto glass   = make_shared<Dielectric>(1.5);
    auto light   = make_shared<DiffuseLight>(Colour(48, 48, 48));


    world.add(make_shared<Sphere>(Point3( 0, 0,-1), 0.5, b_ball));
    world.add(make_shared<Sphere>(Point3(-1, 0,-1), 0.5, glass));
    world.add(make_shared<Sphere>(Point3( 1, 0,-1), 0.5, globe));
    world.add(make_shared<Sphere>(Point3( 2, 2, 0), 0.2, light));
    world.add(make_shared<Sphere>(Point3(-3, 2, -3), 0.2, light));
    world.add(make_shared<Sphere>(Point3( 0,-100.5,-1), 100, checker));
    world.add(make_shared<MovingSphere>(Point3( 2, 0, -1), 0.5, Vec3(0, 0.5, 0), steel));

    world.add(make_shared<ObjModel>("../glass.obj.obj", glass));
    //world.add(make_shared<Triangle>(Point3(0, 0, 0), Point3(1, 0, 0), Point3(1, 1, 0), red));

    return world;
}

HittableList cornellBoxScene(){
    using std::make_shared;

    auto world = HittableList();

    auto red_tex   = make_shared<SolidColour>(Colour(.65, .05, .05));
    auto white_tex = make_shared<SolidColour>(Colour(.73, .73, .73));
    auto green_tex = make_shared<SolidColour>(Colour(.12, .45, .15));
    auto light_tex = make_shared<SolidColour>(Colour(15, 15, 15));

    using Lambertian = Lambertian<false>;
    auto red   = make_shared<Lambertian>(red_tex);
    auto white = make_shared<Lambertian>(white_tex);
    auto green = make_shared<Lambertian>(green_tex);
    auto light = make_shared<DiffuseLight>(light_tex);
    auto glass = make_shared<Dielectric>(1.5);

    world.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    world.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    world.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
    world.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    world.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.add(box1);

    std::shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
    //world.add(box2);

    world.add(make_shared<Translate>(make_shared<ObjModel>("../wine_glass.obj.obj", glass, 50), Vec3(130, 0, 65)));

    return world;
}

HittableList cornellSmoke() {
    using std::make_shared;

    HittableList world;

    auto red_tex   = make_shared<SolidColour>(Colour(.65, .05, .05));
    auto white_tex = make_shared<SolidColour>(Colour(.73, .73, .73));
    auto green_tex = make_shared<SolidColour>(Colour(.12, .45, .15));
    auto light_tex = make_shared<SolidColour>(Colour(7, 7, 7));

    using Lambertian = Lambertian<false>;
    auto red   = make_shared<Lambertian>(red_tex);
    auto white = make_shared<Lambertian>(white_tex);
    auto green = make_shared<Lambertian>(green_tex);
    auto light = make_shared<DiffuseLight>(light_tex);

    world.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    world.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    world.add(make_shared<XZRect>(113, 443, 127, 432, 554, light));
    world.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    world.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0,0,0), Point3(165,330,165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265,0,295));

    std::shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0,0,0), Point3(165,165,165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130,0,65));

    world.add(make_shared<ConstantMedium>(box1, 0.01, make_shared<SolidColour>(Colour(0,0,0))));
    world.add(make_shared<ConstantMedium>(box2, 1000, make_shared<SolidColour>(Colour(1,1,1))));

    return world;
}

HittableList finalScene() {
    using std::make_shared;

    using Lambertian = Lambertian<false>;
    auto ground = make_shared<Lambertian>(make_shared<SolidColour>(Colour(0.48, 0.83, 0.53)));

    auto boxes1 = HittableList();
    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomFloatT(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<Box>(Point3(x0,y0,z0), Point3(x1,y1,z1), ground));
        }
    }

    HittableList objects;
    objects.add(make_shared<BVH_Node>(boxes1, 0, 1));

    auto light = make_shared<DiffuseLight>(Colour(7, 7, 7));
    objects.add(make_shared<XZRect>(123, 423, 147, 412, 554, light));

    objects.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    objects.add(make_shared<Sphere>(
            Point3(0, 150, 145), 50, make_shared<Metal>(make_shared<SolidColour>(Colour(0.8, 0.8, 0.9)), 1.0)
                    ));

    auto boundary = make_shared<Sphere>(Point3(360,150,145), 70, make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<ConstantMedium>(boundary, 0.2, make_shared<SolidColour>(Colour(0.2, 0.4, 0.9))));
    boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    objects.add(make_shared<ConstantMedium>(boundary, .0001, make_shared<SolidColour>(Colour(1,1,1))));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>(Image::readFromFile("../earthmap.jpg")));
    objects.add(make_shared<Sphere>(Point3(400,200,400), 100, emat));

    auto boxes2 = HittableList();
    auto white = make_shared<Lambertian>(make_shared<SolidColour>(Colour(.73, .73, .73)));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(Point3::random(0,165), 10, white));
    }

    objects.add(make_shared<Translate>(
            make_shared<RotateY>(make_shared<BVH_Node>(boxes2, 0, 1), 15),
            Vec3(-100,270,395))
    );

    return objects;
}

Scene testScene() {
    using std::make_shared;

    auto red_tex   = make_shared<SolidColour>(Colour(.65, .05, .05));
    auto white_tex = make_shared<SolidColour>(Colour(.73, .73, .73));
    auto green_tex = make_shared<SolidColour>(Colour(.12, .45, .15));
    auto light_tex = make_shared<SolidColour>(Colour(15, 15, 15));

    using Lambertian = Lambertian<false>;
    auto red   = make_shared<Lambertian>(red_tex);
    auto white = make_shared<Lambertian>(white_tex);
    auto green = make_shared<Lambertian>(green_tex);
    auto light = make_shared<DiffuseLight>(light_tex);
    auto glass = make_shared<Dielectric>(1.5);

    std::shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));

    auto wine_glass = make_shared<Translate>(make_shared<ObjModel>("../wine_glass.obj.obj", glass, 50), Vec3(130, -15, 65));

    return Scene {
        .world = {
            make_shared<YZRect>(0, 555, 0, 555, 555, green),
            make_shared<YZRect>(0, 555, 0, 555, 0, red),
            make_shared<XZRect>(213, 343, 227, 332, 554, light),
            make_shared<XZRect>(0, 555, 0, 555, 0, white),
            make_shared<XZRect>(0, 555, 0, 555, 555, white),
            make_shared<XYRect>(0, 555, 0, 555, 555, white),
            box1,
            wine_glass
        },
        .backgroundColour = Colour(0, 0, 0),
        .cameraBuilder = {
            .lookFrom = Point3(278, 278, -800),
            .lookAt   = Point3(278, 278, 0),
            .fov      = 40,
            .aperture = 1.0 / 16.0,
        }
    };
}

int main() {
    const auto width  = kDebug ? 360 : 800;
    const auto height = kDebug ? 360 : 800;
    const auto samples_per_pixel = kDebug ? 16 : 40;
    const auto max_depth = kDebug ? 50 : 25;

    auto image = Image(width, height);
    auto scene = testScene();

    auto timer = Timer();
    renderMultiThread(4, scene, image, samples_per_pixel, max_depth);
    std::clog << "Finished in " << timer.elapsed() << " seconds";

    image.writeToFile("out.jpg");
};