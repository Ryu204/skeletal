#pragma once

#include "type.hpp"

namespace ske {

struct contour {
    edge_loop exterior;
    std::vector<edge_loop> interiors;
    void fetch_from(const sf::Image& image);
};

} // namespace ske