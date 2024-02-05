#pragma once

#include <cassert>

#include <SFML/Graphics.hpp>

namespace ske {

struct indexer {
    private:
        unsigned int width;
        unsigned int height;
    public:
        indexer(unsigned int width, unsigned int height) : width(width), height(height) {
            assert(width > 0 && height > 0);
        }
        unsigned int encode(unsigned int w, unsigned int h) const {
            assert(w < width && h < height);
            return h * width + w;
        }
        unsigned int encode(const sf::Vector2u& vec) const {
            return encode(vec.x, vec.y);
        }
        sf::Vector2u decode(unsigned int t) const {
            assert(t < width * height);
            return sf::Vector2u{t % width, t / height};
        }
        unsigned int left(unsigned int t, unsigned int count = 1) const {
            assert(t < width * height && t % width - count >= 0);
            return t - count;
        }
        unsigned int right(unsigned int t, unsigned int count = 1) const {
            assert(t < width * height && t % width + count < width);
            return t + count;
        }
        unsigned int up(unsigned int t, unsigned int count = 1) const {
            assert(t < width * height && t >= count * width);
            return t - width * count;
        }
        unsigned int down(unsigned int t, unsigned int count = 1) const {
            assert(t + count * width < width * height);
            return t + count * width;
        }
        unsigned int all(unsigned int t, int down = 0, int right = 0) const {
            if (down <= 0) 
                t = up(t, -down);
            else
                t = this->down(t, down);
            if (right <= 0)
                t = left(t, -right);
            else
                t = this->right(t, right);
            return t;
        }
};

} // namespace ske