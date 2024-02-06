#pragma once

#include <unordered_set>

#include "type.hpp"
#include "indexer.hpp"

namespace ske {

/// @brief extends each side of an image by 1 unit and provide indexer
class silhouette {
        std::unordered_set<indexed_point> m_points;
    public:
        const sf::Vector2u size;
        const indexer index;

        silhouette(const sf::Image& image) 
            : size{image.getSize() + sf::Vector2u{2, 2}}
            , index{size}
        {
            for (auto col = 0u; col < size.x - 2; ++col) 
                for (auto row = 0u; row < size.y - 2; ++row) 
                    if (image.getPixel({col, row}).a > 0)
                        m_points.insert(index.encode(col + 1, row + 1));
        }
        bool contains(indexed_point p) const {
            return m_points.contains(p);
        }
};

} // namespace ske