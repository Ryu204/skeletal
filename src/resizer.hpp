#pragma once

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace ske {

class resizer {
        sf::RenderTexture m_canvas;
        sf::Shader m_resize_shader;
        const sf::Texture& m_old_texture;
    public:
        resizer(sf::Texture& target, unsigned int new_width) 
            : m_old_texture(target)
        {
            if (!m_canvas.create({new_width, 1 + target.getSize().y * new_width / target.getSize().y})) 
                throw std::runtime_error{"Cannot create resize canvas"};
            if (!m_resize_shader.loadFromFile("rsc/resize.frag", sf::Shader::Type::Fragment))
                throw std::runtime_error{"Bad resize shader"};
        }
        const sf::Texture& process(float alpha_threshold = 0.F) {
            const auto chunk_size = sf::Glsl::Vec2{
                std::ceil(m_old_texture.getSize().x * 1.F / m_canvas.getSize().x),
                std::ceil(m_old_texture.getSize().y * 1.F / m_canvas.getSize().y),
            };
            m_resize_shader.setUniform("texture", m_old_texture);
            m_resize_shader.setUniform("chunk_size", chunk_size);
            m_resize_shader.setUniform("texture_size", sf::Glsl::Vec2{m_old_texture.getSize()});
            m_resize_shader.setUniform("output_size", sf::Glsl::Vec2{m_canvas.getSize()});
            m_resize_shader.setUniform("threshold", alpha_threshold);
            m_canvas.clear(sf::Color::Transparent);
            sf::RectangleShape rect{sf::Vector2f{m_canvas.getSize()}};
            rect.setTexture(&m_old_texture);
            m_canvas.draw(rect, sf::RenderStates{&m_resize_shader});
            m_canvas.display();
            return m_canvas.getTexture();
        }
        static sf::Texture with_width(sf::Texture& target, unsigned int width, float alpha_threshold = 0.F) {
            return resizer{target, width}.process(alpha_threshold);
        }
};

} // namespace ske