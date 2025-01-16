cmake_minimum_required(VERSION 3.29)
project(AHRay)

set(CMAKE_CXX_STANDARD 20)

add_executable(AHRay
        src/main.cpp
        include/fragment.h
        include/vector3.h
        src/config.h
        include/ray.h
        include/camera.h)

target_include_directories(AHRay
        PRIVATE include
        PRIVATE src)

target_link_directories(AHRay
        PRIVATE include/fragment.h)
