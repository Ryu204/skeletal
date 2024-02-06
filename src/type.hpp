#pragma once

#include <SFML/Graphics.hpp>

#include "indexer.hpp"

namespace ske {

using indexed_point = unsigned int;
/// @brief point with `.5` fractional part of both x and y components, defined by 2 adjacent indexed cells
using indexed_half_point = sf::Vector2<indexed_point>;
using indexed_edge = sf::Vector2<indexed_half_point>;
using indexed_edge_list = std::vector<indexed_edge>;
using indexed_edge_loop = std::vector<indexed_half_point>;
using edge_loop = std::vector<sf::Vector2f>;

inline sf::Vector2f from_indexed_half_point(const indexed_half_point& p, const indexer& index) {
    return sf::Vector2f{index.decode(p.x) + index.decode(p.y)} / 2.F + sf::Vector2f{0.5F, 0.5F};
}

} // namespace ske