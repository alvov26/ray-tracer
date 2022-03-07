cmake_minimum_required(VERSION 3.21)

# set the project name and version
project(yet_another_ray_tracer VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(yet_another_ray_tracer
        main.cpp
        external/stb_image_write.h
        src/Image.cpp src/Image.h
        src/Utility.h
        src/Vec3.h
        src/Ray.h
        src/Primitives/Sphere.cpp src/Primitives/Sphere.h
        src/Primitives/Hittable.h
        src/Primitives/HittableList.cpp src/Primitives/HittableList.h
        src/Camera.h
        src/Material.h
        src/Timer.h
        src/Primitives/MovingSphere.cpp src/Primitives/MovingSphere.h
        src/AABB.h
        src/Primitives/BVH_Node.cpp src/Primitives/BVH_Node.h
        src/Texture.h
        external/stbi_image.h
        src/Primitives/Rect.cpp src/Primitives/Rect.h
        src/Primitives/Box.cpp src/Primitives/Box.h
        src/Primitives/Translate.cpp src/Primitives/Translate.h
        src/Primitives/Rotate.cpp src/Primitives/Rotate.h
        src/Primitives/ObjModel.cpp src/Primitives/ObjModel.h
        external/tiny_obj_loader.h external/tiny_obj_loader.cpp
        src/ConstantMedium.cpp src/ConstantMedium.h src/Scene.h src/Scene.cpp)

set_source_files_properties(
        "src/tiny_obj_loader.h src/tiny_obj_loader.cpp src/stbi_image.h src/stb_image_write.h"
        PROPERTIES
        COMPILE_FLAGS "-w"
)

target_compile_options(yet_another_ray_tracer PRIVATE -Wall -Wextra -Wpedantic)

# Scary flag. It enables ALL warnings. Now imagine -Werror...
# target_compile_options(yet_another_ray_tracer PRIVATE -Weverything)