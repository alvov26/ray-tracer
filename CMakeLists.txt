cmake_minimum_required(VERSION 3.21)

# set the project name and version
project(yet_another_ray_tracer VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(yet_another_ray_tracer
        main.cpp
        src/stb_image_write.h
        src/Image.cpp src/Image.h
        src/Utility.h
        src/Vec3.cpp src/Vec3.h
        src/Ray.h
        src/Sphere.cpp src/Sphere.h
        src/Hittable.h
        src/HittableList.cpp src/HittableList.h
        src/Camera.h
        src/Material.h
        src/Timer.h
        src/MovingSphere.cpp src/MovingSphere.h
        src/AABB.h
        src/BVH_Node.cpp src/BVH_Node.h
        src/Texture.h
        src/stbi_image.h
        src/Rect.cpp src/Rect.h
        src/Box.cpp src/Box.h
        src/Translate.cpp src/Translate.h
        src/Rotate.cpp src/Rotate.h)

target_compile_options(yet_another_ray_tracer PRIVATE -Wall -Wextra -Wpedantic)

# Scary flag. It enables ALL warnings. Now imagine -Werror...
# target_compile_options(yet_another_ray_tracer PRIVATE -Weverything)