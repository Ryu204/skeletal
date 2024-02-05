#include <iostream>

#include "resizer.hpp"

void draw_texture(sf::RenderTarget& target, const sf::Texture& texture, float transparency = 1.F) {
    sf::RectangleShape rect{sf::Vector2f{target.getSize()}};
    rect.setTexture(&texture);
    rect.setFillColor(sf::Color(255, 255, 255, 255 * transparency));
    target.draw(rect);
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) 
            throw std::runtime_error{"Provide path to image"};
        sf::Texture target;
        if (!target.loadFromFile(argv[1])) 
            throw std::runtime_error{"Check path to image"};
        const auto resized_texture = ske::resizer::with_width(target, 100.F);
        if (!resized_texture.copyToImage().saveToFile("img/test_resize.png")) 
            throw std::runtime_error{"Cannot save resize result"};
        sf::RenderTexture result;
        if (!result.create(target.getSize()))
            throw std::runtime_error{"Cannot create result canvas"};
        result.clear(sf::Color::Red);
        draw_texture(result, resized_texture);
        draw_texture(result, target);
        result.display();
        if (!result.getTexture().copyToImage().saveToFile("img/test_final.png"))
            throw std::runtime_error{"Cannt save result image"};
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return -2;
    }
}