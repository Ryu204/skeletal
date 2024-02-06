#include "marcher.hpp"
#include "silhouette.hpp"

namespace ske {

auto march(const silhouette& sil) {
    std::unordered_map<indexed_point, indexed_edge_list> res;
    for (auto col = 0u; col < sil.size.x - 1; ++col)
        for (auto row = 0u; row < sil.size.y - 1; ++row) {
            const auto current = sil.index.encode(col, row);
            std::uint8_t bitmask = 0;
            indexed_point bitCell;
            if (sil.contains(current)) {
                bitmask |= 1;
                bitCell = current;
            }
            if (sil.contains(sil.index.right(current))) {
                bitmask |= 2;
                bitCell = sil.index.right(current);
            }
            if (sil.contains(sil.index.all(current, 1, 1))) {
                bitmask |= 4;
                bitCell = sil.index.all(current, 1, 1);
            }
            if (sil.contains(sil.index.down(current))) {
                bitmask |= 8;
                bitCell = sil.index.down(current);
            }
            if (bitmask == 0)
                continue;
            /*
                /\     a b
                \/     c d
                __ |   x y
            */
            const indexed_half_point up{current, sil.index.right(current)};
            const indexed_half_point left{current, sil.index.down(current)};
            const indexed_half_point right{sil.index.right(current), sil.index.all(current, 1, 1)};
            const indexed_half_point down{sil.index.down(current), sil.index.all(current, 1, 1)};
            const indexed_edge a{up, left};
            const indexed_edge b{up, right};
            const indexed_edge c{left, down};
            const indexed_edge d{right, down};
            const indexed_edge x{left, right};
            const indexed_edge y{up, down};
            switch (bitmask) {
                case 0:
                    break;
                case 1:
                    res[bitCell].push_back(a);
                    break;
                case 2:
                    res[bitCell].push_back(b);
                    break;
                case 4:
                    res[bitCell].push_back(d);
                    break;
                case 8:
                    res[bitCell].push_back(c);
                    break;
                case 1 | 2:
                    res[bitCell].push_back(x);
                    break;
                case 1 | 4:
                    res[bitCell].push_back(b);
                    res[bitCell].push_back(c);
                    break;
                case 1 | 8:
                    res[bitCell].push_back(y);
                    break;
                case 2 | 4:
                    res[bitCell].push_back(y);
                    break;
                case 2 | 8:
                    res[bitCell].push_back(a);
                    res[bitCell].push_back(d);
                    break;
                case 4 | 8:
                    res[bitCell].push_back(x);
                    break;
                case 1 | 2 | 4:
                    res[bitCell].push_back(c);
                    break;
                case 1 | 2 | 8:
                    res[bitCell].push_back(d);
                    break;
                case 1 | 4 | 8:
                    res[bitCell].push_back(b);
                    break;
                case 2 | 4 | 8:
                    res[bitCell].push_back(a);
                    break;
                case 1 | 2 | 4 | 8:
                    break;
                default:
                    assert(false && "Unreachable");
            }
        }
    return std::move(res);
}

/// @brief Convert a collection of edges in loops into a collection of loops
std::vector<indexed_edge_loop> edge_lists_to_loops(const indexed_edge_list& list) {
    std::vector<indexed_edge_loop> res;
    std::unordered_map<indexed_half_point, std::vector<indexed_half_point>, indexed_half_point_hash> neighbors;
    std::unordered_map<indexed_half_point, bool, indexed_half_point_hash> visited;
    for (const auto& edge : list) {
        neighbors[edge.x].push_back(edge.y);
        neighbors[edge.y].push_back(edge.x);
        visited[edge.x] = false;
        visited[edge.y] = false;
    }
    for (const auto& [start_point, _] : visited) {
        if (visited[start_point])
            continue;
        auto next = start_point;
        indexed_edge_loop start_point_loop = {start_point};
        while (!visited[next]) {
            visited[next] = true;
            assert(neighbors[next].size() == 2);
            const auto& next_itr = std::find_if(neighbors[next].cbegin(), neighbors[next].cend(), 
                [&visited](const auto& x) { return !visited[x]; });
            const auto adjacent_all_visited = next_itr == neighbors[next].end();
            if (adjacent_all_visited)
                break;
            next = *next_itr;
            start_point_loop.emplace_back(next);
        }
        res.emplace_back(std::move(start_point_loop));
    }
    return res;
}

edge_loop indexed_loop_to_loop(const indexed_edge_loop& loop, const silhouette& sil) {
    edge_loop res;
    static const auto offset = [&sil](const indexed_half_point& p) { return 
        (sf::Vector2f{sil.index.decode(p.x) + sil.index.decode(p.y)} / 2.F - sf::Vector2f{0.5F, 0.5F})
        .cwiseDiv(sf::Vector2f{sil.size - sf::Vector2u{2, 2}}); 
    };
    for (const auto& half_point : loop) {
        res.emplace_back(offset(half_point));
    }
    return res;
}

void contour::fetch_from(const sf::Image& image) {
    exterior.clear();
    interiors.clear();
    const silhouette sil{image};
    auto adjacent_of{march(sil)};
    for (const auto& leader : sil.regions()) {
        if (!adjacent_of.contains(leader)) 
            adjacent_of[leader];
    }
    for (const auto& [cell, edges_list] : adjacent_of) {
        const auto leader = sil.region_of(cell);
        if (cell == leader) 
            continue;
        adjacent_of[leader].insert(adjacent_of[leader].end(), edges_list.begin(), edges_list.end());
    }
    for (const auto& leader : sil.regions()) {
        auto edge_loops = edge_lists_to_loops(adjacent_of[leader]);
        assert(!edge_loops.empty());
        for (const auto& loop : edge_loops) 
            interiors.emplace_back(indexed_loop_to_loop(loop, sil));
        static const auto& width = [](const edge_loop& loop) {
            return std::max_element(loop.cbegin(), loop.cend(), [](const auto& x, const auto& y) { return x.x < y.x; })->x
            - std::min_element(loop.cbegin(), loop.cend(), [](const auto& x, const auto& y) { return x.x < y.x; })->x;
        };     
        const auto& widest_loop = std::max_element(interiors.cbegin(), interiors.cend(), 
            [](const auto& a, const auto& b) { return width(a) < width(b); });
        assert(widest_loop != interiors.end());
        const auto exterior_index = widest_loop - interiors.cbegin();
        exterior.swap(interiors[exterior_index]);
        interiors.erase(widest_loop);
    }
}

} // namespace ske