#include <iostream>
#include <fstream>

#include "resizer.hpp"
#include "marcher.hpp"
#include "mesh.hpp"
#include "debug.hpp"
#include "arg.hpp"

int main(int argc, char* argv[]) {
    try {
        argparse::ArgumentParser parser;
        ske::build_parser(parser);
        try {
            parser.parse_args(argc, argv);
        } catch (std::exception& e) {
            std::cerr << "Invalid use\n";
            std::cerr << parser;
            return -1;
        }
        sf::Texture target;
        if (!target.loadFromFile(parser.get("file"))) 
            throw std::runtime_error{"Check path to image"};
        const auto resized_texture = ske::resizer::with_width(target, parser.get<unsigned int>("--width"));
        const auto resized_image = resized_texture.copyToImage();
        ske::contour contours;
        contours.fetch_from(resized_image);
        ske::mesh mesh;
        mesh.from_contour(contours);
        if (parser.get<bool>("--debug")) {
            ske::save_debug_files(ske::debug_info{
                .original = target,
                .resized_texture = resized_texture,
                .bounds = contours,
                .mesh_info = mesh,
            });
        }
        const auto output = mesh.dump();
        std::ios::sync_with_stdio(false);
        std::cout.tie(nullptr);
        if (parser.get<bool>("--stdout")) {
            std::cout << output << "\n";
        }
        if (parser.is_used("--output")) {
            std::ofstream writer{parser.get("--output")};
            writer << output << "\n";
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