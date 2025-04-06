cmake_minimum_required(VERSION 3.30)
project(BTP)

set(CMAKE_CXX_STANDARD 20)

add_executable(BTP main.cpp
        vec3.h
        color.h
        ray.h)
