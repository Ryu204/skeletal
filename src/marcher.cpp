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

void contours::fetch_from(const sf::Image& image) {
    exteriors.clear();
    interiors.clear();
    const silhouette sil{image};
    const auto adjacent_of{march(sil)};
    
}

} // namespace ske