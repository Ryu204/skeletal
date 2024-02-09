cmake_minimum_required(VERSION 3.24)

project(skeletal LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory("./extern/SFML")
add_subdirectory("./extern/TQMesh")
add_subdirectory("./extern/argparse")

file(GLOB_RECURSE 
    skeletal_source_code
    CONFIGURE_DEPENDS
    "./src/*.?pp")

add_executable(skeletal ${skeletal_source_code})
target_include_directories(skeletal PRIVATE ./extern/TQMesh/src/algorithm)
target_link_libraries(skeletal PRIVATE sfml-graphics algorithm argparse::argparse)