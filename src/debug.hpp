#pragma once

#include <SFML/Graphics.hpp>

#include "marcher.hpp"
#include "mesh.hpp"

namespace ske {

struct debug_setting {
    unsigned int image_width{1000};
    bool save_resize{true};
    bool save_contours{true};
    bool save_mesh{true};
    bool save_all{true};
};

struct debug_info {
    const sf::Texture& original;
    const sf::Texture& resized_texture;
    const contour& bounds;
    const mesh& mesh;
};

void save_debug_files(const debug_info& info, const debug_setting& settings = debug_setting{});

} // namespace ske