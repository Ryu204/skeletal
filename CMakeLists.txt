cmake_minimum_required(VERSION 3.24)

project(skeletal LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE 
    skeletal_source_code
    CONFIGURE_DEPENDS
    "./src/*.?pp"
)

if (SKELETAL_PYTHON_MODULE)
    add_subdirectory("./extern/pybind11")
    list(APPEND skeletal_source_code "./python/bind.cpp")
    pybind11_add_module(skeletal ${skeletal_source_code})
    if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
        add_compile_options("-fPIC")
    endif()
else()
    add_executable(skeletal ${skeletal_source_code})
endif()

add_subdirectory("./extern/SFML")
add_subdirectory("./extern/TQMesh")
add_subdirectory("./extern/argparse")
add_subdirectory("./extern/json")

target_include_directories(skeletal PRIVATE ./extern/TQMesh)
target_link_libraries(skeletal PRIVATE 
    sfml-graphics 
    algorithm 
    argparse::argparse 
    nlohmann_json::nlohmann_json
)