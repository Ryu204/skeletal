#include <iostream>

#include "resizer.hpp"
#include "marcher.hpp"
#include "mesh.hpp"
#include "debug.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) 
            throw std::runtime_error{"Provide path to image"};
        sf::Texture target;
        if (!target.loadFromFile(argv[1])) 
            throw std::runtime_error{"Check path to image"};
        const auto resized_texture = ske::resizer::with_width(target, 15.F);
        const auto resized_image = resized_texture.copyToImage();
        ske::contour contours;
        contours.fetch_from(resized_image);
        ske::mesh mesh;
        mesh.from_contour(contours);
        auto debug = true;
        if (debug) {
            ske::save_debug_files(ske::debug_info{
                .original = target,
                .resized_texture = resized_texture,
                .bounds = contours,
                .mesh = mesh,
            });
        }
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return -2;
    }
}