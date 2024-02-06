#pragma once

#include "type.hpp"

namespace ske {

struct contours {
    std::vector<edge_loop> exteriors;
    std::vector<edge_loop> interiors;
    void fetch_from(const sf::Image& image);
};

} // namespace ske