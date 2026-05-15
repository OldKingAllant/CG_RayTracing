#include "obj_loader.hpp"
#include <string_view>

namespace geo = cg_raytracing::geometry;
std::expected<geo::Mesh, std::string>
geo::LoadFromObj(std::filesystem::path _obj_path) {
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

        for (auto part : std::views::split(line, ' ')) {
            switch (state) {
            case 0: {
                std::string pattern{std::string_view(part)};
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
            }
            case 1:
                // TODO:append to vertex array
                break;
            case 2:
                // TODO:append to normal array
                break;
            case 3:
                // TODO:append to uv coordinate array
                break;
            case 4:
                // TODO:smooth shading indicator
                break;
            case 5:
                // TODO:append to faces array
                break;
            }
        }
    }
}
