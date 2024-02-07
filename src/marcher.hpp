#pragma once

#include "type.hpp"

namespace ske {

struct contour {
    struct region {
        edge_loop exterior;
        std::vector<edge_loop> interiors;
    };
    std::vector<region> regions;
    void fetch_from(const sf::Image& image);
};

} // namespace ske