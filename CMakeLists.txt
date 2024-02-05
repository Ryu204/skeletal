cmake_minimum_required(VERSION 3.24)

project(skeletal LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory("./extern/SFML")

file(GLOB_RECURSE 
    skeletal_source_code
    CONFIGURE_DEPENDS
    "./src/*.?pp")

add_executable(skeletal ${skeletal_source_code})