#pragma once
#include "vec3.hpp"

namespace cg_raytracing::geometry {

// Supported shading models
enum class MaterialType {
    DIFFUSE,
    METAL,
    DIELECTRIC
};

struct Material {
    cg_raytracing::math::Vec3 m_albedo;                      // base color (r, g, b) in [0, 1]
    MaterialType              m_type = MaterialType::DIFFUSE;
    float                     m_roughness = 1.0f;            // METAL only: 0 = mirror, 1 = rough
    float                     m_ior       = 1.5f;            // DIELECTRIC only: index of refraction (glass ≈ 1.5)

    // Factory helpers — prefer these over direct construction
    static Material Diffuse(cg_raytracing::math::Vec3 _color) {
        return { _color, MaterialType::DIFFUSE };
    }

    static Material Metal(cg_raytracing::math::Vec3 _color, float _roughness = 0.0f) {
        return { _color, MaterialType::METAL, _roughness };
    }

    // White dielectric by default (glass-like)
    static Material Dielectric(float _ior = 1.5f) {
        return { {1, 1, 1}, MaterialType::DIELECTRIC, 0.0f, _ior };
    }
};

} // namespace cg_raytracing::geometry