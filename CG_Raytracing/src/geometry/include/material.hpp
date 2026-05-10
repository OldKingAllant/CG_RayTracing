#pragma once
#include "vec3.hpp"

namespace cg_raytracing::geometry {

enum class MaterialType {
    DIFFUSE,
    METAL,
    DIELECTRIC
};

struct Material {
    cg_raytracing::math::Vec3 albedo;   // base color (r, g, b) in [0, 1]
    MaterialType type = MaterialType::DIFFUSE;

    float roughness = 1.0f;   // for METAL: 0 = mirror, 1 = rough
    float ior       = 1.5f;   // for DIELECTRIC: index of refraction (glass ≈ 1.5)

    // Factory helpers
    static Material Diffuse(cg_raytracing::math::Vec3 color) {
        return { color, MaterialType::DIFFUSE };
    }

    static Material Metal(cg_raytracing::math::Vec3 color, float roughness = 0.0f) {
        return { color, MaterialType::METAL, roughness };
    }

    static Material Dielectric(float ior = 1.5f) {
        return { {1, 1, 1}, MaterialType::DIELECTRIC, 0.0f, ior };
    }
};

} // namespace cg_raytracing::geometry