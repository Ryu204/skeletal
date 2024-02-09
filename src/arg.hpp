#pragma once

#include <argparse/argparse.hpp>

namespace ske {

inline void build_parser(argparse::ArgumentParser& parser) {
    parser.add_argument("--width", "-w")
        .help("number of horizontal pixels for marching squares algorithm")
        .default_value(10u)
        .scan<'u', unsigned int>();
    parser.add_argument("--debug", "-d")
        .help("save debug images")
        .flag();
    parser.add_argument("file")
        .help("path to image file")
        .required();
}

} // namespace ske