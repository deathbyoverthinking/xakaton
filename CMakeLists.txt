cmake_minimum_required(VERSION 3.29)
project(xakaton)

set(CMAKE_CXX_STANDARD 20)

add_executable(xakaton main.cpp
)

add_executable(main main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")