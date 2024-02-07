#include <iostream>

#include "resizer.hpp"
#include "marcher.hpp"

void draw_texture(sf::RenderTarget& target, const sf::Texture& texture, float transparency = 1.F) {
    sf::RectangleShape rect{sf::Vector2f{target.getSize()}};
    rect.setTexture(&texture);
    rect.setFillColor(sf::Color(255, 255, 255, 255 * transparency));
    target.draw(rect);
}

void draw_contours(sf::RenderTarget& target, const ske::contour& contours) {
    const auto size = sf::Vector2f{target.getSize()};
    for (const auto& region : contours.regions) {
        static constexpr std::array<sf::Color, 4> ecs = {sf::Color::Magenta, sf::Color::Cyan, sf::Color::Blue, sf::Color::Black};
        static constexpr std::array<sf::Color, 4> ics = {sf::Color::Green, sf::Color::Red, sf::Color::Yellow, sf::Color{100, 100, 100}};
        const auto ec = ecs[rand() % 4];
        const auto ic = ics[rand() % 4];
        sf::VertexArray exterior{sf::PrimitiveType::LineStrip};
        std::vector<sf::VertexArray> interiors;
        for (const auto& point : region.exterior) {
            exterior.append(sf::Vertex{point.cwiseMul(size), ec});
        }
        target.draw(exterior);
        for (const auto& loop : region.interiors) {
            interiors.push_back(sf::VertexArray{sf::PrimitiveType::LineStrip});
            for (const auto& point : loop) {
                interiors.back().append(sf::Vertex{point.cwiseMul(size), ic});
            }
            target.draw(interiors.back());
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) 
            throw std::runtime_error{"Provide path to image"};
        sf::Texture target;
        if (!target.loadFromFile(argv[1])) 
            throw std::runtime_error{"Check path to image"};
        const auto resized_texture = ske::resizer::with_width(target, 30.F);
        const auto resized_image = resized_texture.copyToImage();
        if (!resized_image.saveToFile("img/test_resize.png")) 
            throw std::runtime_error{"Cannot save resize result"};
        ske::contour contours;
        contours.fetch_from(resized_image);
        sf::RenderTexture result;
        if (!result.create(target.getSize()))
            throw std::runtime_error{"Cannot create result canvas"};
        result.clear(sf::Color::Transparent);
        draw_texture(result, resized_texture);
        draw_texture(result, target);
        draw_contours(result, contours);
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