cmake_minimum_required(VERSION 3.29)
project(AHRay)

set(CMAKE_CXX_STANDARD 20)

add_executable(AHRay
        src/main.cpp
        include/fragment.h
        include/vector3.h
        src/config.h
        include/ray.h
        include/camera.h
        include/viewport.h
        include/scene.h
        include/sphere.h
        include/hitrecord.h
        include/material.h
        include/cylinder.h
        include/ahray_objloader.h
        include/triangle.h
        include/pointlight.h
)

target_include_directories(AHRay
        PRIVATE include
        PRIVATE src)

target_link_directories(AHRay
        PRIVATE include/fragment.h)
