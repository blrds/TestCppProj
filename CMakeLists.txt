cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(TestProj)

find_package(libhv REQUIRED)

add_executable(main main.cpp)

add_library(circle
    OBJECT
        Circle.h
        Circle.cpp
)

target_compile_options(circle
    PRIVATE
        -Wall -fPIC
)

add_library(circle-a
    STATIC
        $<TARGET_OBJECTS:circle>
)
target_link_libraries(main circle-a)
target_link_libraries(main ~/Desktop/TestProj/libhv/build/lib/libhv_static.a)