#include "debug.hpp"

namespace ske {

void draw_texture(sf::RenderTarget& target, const sf::Texture& texture, float transparency = 1.F) {
    sf::RectangleShape rect{sf::Vector2f{target.getSize()}};
    rect.setTexture(&texture);
    rect.setFillColor(sf::Color(255, 255, 255, 255 * transparency));
    target.draw(rect);
}

void draw_contours(sf::RenderTarget& target, const ske::contour& contours) {
    const auto size = sf::Vector2f{target.getSize()};
    for (const auto& region : contours.regions) {
        static constexpr std::array<sf::Color, 4> ecs = {sf::Color::Magenta, sf::Color::Cyan, sf::Color{0xEAA22200}, sf::Color{0x666666FF}};
        static constexpr std::array<sf::Color, 4> ics = {sf::Color::Green, sf::Color::Red, sf::Color::Blue, sf::Color::Black};
        const auto ec = ecs[rand() % 4];
        const auto ic = ics[rand() % 4];
        sf::VertexArray exterior{sf::PrimitiveType::LineStrip};
        std::vector<sf::VertexArray> interiors;
        for (const auto& point : region.exterior) {
            exterior.append(sf::Vertex{point.cwiseMul(size), ec});
        }
        exterior.append(sf::Vertex{region.exterior.front().cwiseMul(size), ec});
        target.draw(exterior);
        for (const auto& loop : region.interiors) {
            interiors.push_back(sf::VertexArray{sf::PrimitiveType::LineStrip});
            for (const auto& point : loop) {
                interiors.back().append(sf::Vertex{point.cwiseMul(size), ic});
            }
            interiors.back().append(sf::Vertex{loop.front().cwiseMul(size), ic});
            target.draw(interiors.back());
        }
    }
}

void draw_mesh(sf::RenderTarget& target, const mesh& mesh) {
    sf::VertexArray buffer{sf::PrimitiveType::Lines};
    const sf::Vector2f size{target.getSize()};
    assert(mesh.index_buffer.size() % 3 == 0);
    for (auto i = mesh.index_buffer.cbegin(); i != mesh.index_buffer.cend(); i += 3) {
        const auto a = mesh.vertices.at(*i);
        const auto b = mesh.vertices.at(*(i + 1));
        const auto c = mesh.vertices.at(*(i + 2));
        buffer.append(sf::Vertex{a.cwiseMul(size), sf::Color::Blue});
        buffer.append(sf::Vertex{b.cwiseMul(size), sf::Color::Blue});
        buffer.append(sf::Vertex{b.cwiseMul(size), sf::Color::Blue});
        buffer.append(sf::Vertex{c.cwiseMul(size), sf::Color::Blue});
        buffer.append(sf::Vertex{c.cwiseMul(size), sf::Color::Blue});
        buffer.append(sf::Vertex{a.cwiseMul(size), sf::Color::Blue});
    }
    target.draw(buffer);
}

void create_output_dir(const std::filesystem::path& name) {
    if (std::filesystem::is_directory(name)) 
        return;
    if (!std::filesystem::create_directory(name)) 
        throw std::runtime_error{"Cannot create output dir"};
}

void save_debug_files(const debug_info& info, const debug_setting& settings) {
    create_output_dir("output");
    if (settings.save_resize && !info.resized_texture.copyToImage().saveToFile("output/_resized.png"))
        throw std::runtime_error{"Cannot save resized image"};
    sf::RenderTexture canvas;
    const auto original_size = info.original.getSize();
    const sf::Vector2u canvas_size{
        settings.image_width,
        static_cast<unsigned int>(settings.image_width * original_size.y * 1.F / original_size.x)
    };
    if (!canvas.create(canvas_size)) {
        throw std::runtime_error{"Cannot create debug canvas"};
    }
    if (settings.save_contours) {
        canvas.clear(sf::Color::Transparent);
        draw_texture(canvas, info.original);
        draw_contours(canvas, info.bounds);
        canvas.display();
        if (!canvas.getTexture().copyToImage().saveToFile("output/_contours.png"))
            throw std::runtime_error{"Cannot save contour image"};
    }
    if (settings.save_mesh) {
        canvas.clear(sf::Color::Transparent);
        draw_texture(canvas, info.original);
        draw_mesh(canvas, info.mesh_info);
        if (!canvas.getTexture().copyToImage().saveToFile("output/_mesh.png"))
            throw std::runtime_error{"Cannot save mesh image"};
    }
    if (settings.save_all) {
        canvas.clear(sf::Color::Transparent);
        draw_texture(canvas, info.resized_texture, 0.3F);
        draw_texture(canvas, info.original);
        draw_mesh(canvas, info.mesh_info);
        draw_contours(canvas, info.bounds);
        canvas.display();
        if (!canvas.getTexture().copyToImage().saveToFile("output/_all.png"))
            throw std::runtime_error{"Cannot save result image"};
    }
}

} // namespace ske