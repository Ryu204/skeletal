#pragma once

#include <unordered_set>

#include "type.hpp"
#include "indexer.hpp"
#include "dsu.hpp"

namespace ske {

/// @brief extends each side of an image by 1 unit and provide indexer
class silhouette {
    public:
        const sf::Vector2u size;
        const indexer index;

        silhouette(const sf::Image& image)
            : size{image.getSize() + sf::Vector2u{2, 2}}
            , index{size}
            , m_dsu{size.x * size.y}
        {
            fetch_points(image);
            make_dsu();
            for (const auto& probably_leader : m_dsu.leaders())
                if (contains(probably_leader))
                    m_dsu_leaders.insert(probably_leader);
        }
        bool contains(indexed_point p) const {
            return m_points.contains(p);
        }
        const dsu& get_dsu() const {
            return m_dsu;
        }
        const auto& regions() const {
            return m_dsu_leaders;
        }
        indexed_point region_of(indexed_point p) const {
            return m_dsu.find(p);
        }
    private:
        void fetch_points(const sf::Image& image) {
            for (auto col = 0u; col < size.x - 2; ++col) 
                for (auto row = 0u; row < size.y - 2; ++row) 
                    if (image.getPixel({col, row}).a > 0)
                        m_points.insert(index.encode(col + 1, row + 1));
        }
        void make_dsu() {
            /* _ and | and \ connections */
            for (auto col = 0u; col < size.x - 1; ++col)
                for (auto row = 0u; row < size.y - 1; ++row) {
                    const auto current = index.encode(col, row);
                    if (!contains(current))
                        continue;
                    const auto right = index.right(current);
                    if (contains(right))
                        m_dsu.unite(current, right);
                    const auto down = index.down(current);
                    if (contains(down))
                        m_dsu.unite(current, down);
                    const auto dr = index.all(current, 1, 1);
                    if (contains(dr))
                        m_dsu.unite(current, dr);
                }
            /* / connection */
            for (auto col = 1u; col < size.x; ++col)
                for (auto row = 0u; row < size.y - 1; ++row) {
                    const auto current = index.encode(col, row);
                    if (!contains(current))
                        continue;
                    const auto dl = index.all(current, 1, -1);
                    if (contains(dl))
                        m_dsu.unite(current, dl);
                }
        }
        std::unordered_set<indexed_point> m_points;
        dsu m_dsu;
        std::unordered_set<indexed_point> m_dsu_leaders;
};

} // namespace ske