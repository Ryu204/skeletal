#pragma once

#include <nlohmann/json.hpp>

#include "marcher.hpp"

namespace ske {

struct mesh {
        std::vector<sf::Vector2f> vertices;
        std::vector<unsigned int> index_buffer;

        void from_contour(const contour& bounds);
        nlohmann::json dump() const;

    private:
        static void read_from_file(const std::filesystem::path& name, contour::region& reg);
};

} // namespace ske