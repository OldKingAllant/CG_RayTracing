#include "mesh.hpp"

namespace cg_raytracing::geometry {

Mesh::Mesh(cg_raytracing::math::Vec3 _center, Material _material) {
    this->m_center = _center;
    this->m_material = _material;
}
std::optional<HitRecord> Mesh::Hit(const cg_raytracing::math::Ray &_ray,
                                   float _t_min, float _t_max) const {
    return HitRecord();
};

BoundingBox Mesh::GetBoundingBox() const {
    return BoundingBox();
}

std::expected<int, std::string>
Mesh::LoadFromObj(std::filesystem::path _obj_path) {
    std::ifstream obj_file(_obj_path);

    if (!obj_file.is_open()) {
        return std::unexpected("Failed to open file: " + _obj_path.string());
    }

    std::string line;

    while (std::getline(obj_file, line)) {
        // 0 -> check what type of input it is
        // 1 -> (v) vertices
        // 2 -> (vn) vertices normals
        // 3 -> (vt) uv coordinates
        // 4 -> (s) smooth shading
        // 5 -> (f) faces v/vt/vn
        uint8_t state = 0;
        uint8_t info_index = 0;

        for (auto part : std::views::split(line, ' ')) {
            std::string pattern{std::string_view(part)};
            switch (state) {
            case 0:
                if (pattern == "v")
                    state = 1;
                if (pattern == "vn")
                    state = 2;
                if (pattern == "vt")
                    state = 3;
                if (pattern == "s")
                    state = 4;
                if (pattern == "f")
                    state = 5;

                break;
            case 1:
                // TODO:append to vertex array
                if (info_index == 0) {
                    this->m_vertex_positions.push_back(
                        math::Vec3(std::stof(pattern), 0, 0));
                } else if (info_index == 1) {
                    this->m_vertex_positions.back().y = std::stof(pattern);
                } else {
                    this->m_vertex_positions.back().z = std::stof(pattern);
                }
                info_index += 1;
                break;
            case 2:
                // TODO:append to normal array
                if (info_index == 0) {
                    this->m_vertex_normals.push_back(
                        math::Vec3(std::stof(pattern), 0, 0));
                } else if (info_index == 1) {
                    this->m_vertex_normals.back().y = std::stof(pattern);
                } else {
                    this->m_vertex_normals.back().z = std::stof(pattern);
                }
                info_index += 1;
                break;
            case 3:
                // TODO:append to uv coordinate array
                if (info_index == 0) {
                    this->m_face_uv.push_back(
                        std::array<float, 2>{std::stof(pattern), 0});

                } else {
                    this->m_face_uv.back()[1] = std::stof(pattern);
                }
                info_index += 1;
                break;
            case 4: {
                if (pattern == "0")
                    this->m_smooth_shading = false;
                else
                    this->m_smooth_shading = true;
                break;
            }
            case 5:
                size_t count = 0;
                for (auto triangle_index : std::views::split(part, '/')) {
                    std::string string_index{std::string_view(triangle_index)};
                    size_t int_index = std::stoi(string_index);
                    if (count == 0) {
                        this->m_indices.push_back(std::array<size_t, 3>());
                    }

                    this->m_indices.back()[count] = int_index;

                    count += 1;
                }
                break;
            }
        }
    }
    return 0;
}
} // namespace cg_raytracing::geometry
