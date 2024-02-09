#include "mesh.hpp"

#include "MeshGenerator.h"

namespace ske {

void mesh::from_contour(const contour& bounds) {
    vertices.clear();
    index_buffer.clear();

    const TQMesh::TQAlgorithm::UserSizeFunction f_size = [](const auto& x) { return 8.F; };
    TQMesh::TQAlgorithm::Domain domain{ f_size };

    std::vector<TQMesh::TQAlgorithm::Vec2d> ext_bound, int_bound;
    auto reg_count = 0u;
    for (const auto& region : bounds.regions) {
        reg_count++;
        ext_bound.clear();
        static std::array<double, 2> vec;
        for (const auto& p : region.exterior) {
            vec[0] = p.x;
            vec[1] = p.y;
            ext_bound.emplace_back(vec);
        }
        std::vector<int> markers((int)ext_bound.size(), 1);
        domain.add_exterior_boundary().set_shape_from_coordinates(ext_bound, markers);
        for (const auto& interior : region.interiors) {
            int_bound.clear();
            for (const auto& p : interior) {
                vec[0] = p.x;
                vec[1] = p.y;
                int_bound.emplace_back(vec);
            }
            markers.resize(int_bound.size());
            domain.add_interior_boundary().set_shape_from_coordinates(int_bound, markers);
        }
        TQMesh::TQAlgorithm::MeshGenerator generator;
        auto& mesh = generator.new_mesh(domain);
        generator.triangulation(mesh).generate_elements();
        std::unordered_map<unsigned int, unsigned int> algorithm_to_index;
        for (auto i = 0u; i < generator.size(); ++i) {
            auto& mesh = generator.mesh(i);
            for (const auto& vert : mesh.vertices()) {
                vertices.emplace_back(static_cast<float>(vert->xy()[0]), static_cast<float>(vert->xy()[1]));
                algorithm_to_index[vert->index()] = vertices.size() - 1;
            }
            for (const auto& tri : mesh.triangles()) {
                index_buffer.push_back(algorithm_to_index[tri->v1().index()]);
                index_buffer.push_back(algorithm_to_index[tri->v2().index()]);
                index_buffer.push_back(algorithm_to_index[tri->v3().index()]);
            }
        }
    }
}

nlohmann::json mesh::dump() const {
    nlohmann::json res;
    for (const auto& v : vertices) {
        res["vertices"].push_back({{"x", v.x}, {"y", v.y}});
    }
    for (const auto& id : index_buffer) {
        res["indices"].push_back(id);
    }
    return std::move(res);
}

} // namespace ske