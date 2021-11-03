cmake_minimum_required(VERSION 3.21)
project(yet_another_ray_tracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(yet_another_ray_tracer main.cpp src/stb_image_write.h src/Image.cpp src/Image.h src/Utility.h src/Vec3.cpp src/Vec3.h src/Ray.cpp src/Ray.h src/Sphere.cpp src/Sphere.h src/Hittable.h src/HittableList.cpp src/HittableList.h src/Camera.h src/Material.h)
