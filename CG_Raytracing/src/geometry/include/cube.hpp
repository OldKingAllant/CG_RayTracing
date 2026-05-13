#pragma once

#include "hit_record.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "triangle.hpp"
#include "vec3.hpp"
#include <optional>
#include <array>

namespace cg_raytracing::geometry {

class Cube {
public:
    Material                 m_material;
    std::array<Triangle, 12> m_triangles;

    Cube(cg_raytracing::math::Vec3 _center, float _half_size, Material _material);

    std::optional<HitRecord> Hit(const cg_raytracing::math::Ray& _ray,
                                 float _t_min = 0.001f,
                                 float _t_max = 1e9f) const;

private:
    std::array<Triangle, 12> BuildTriangles(cg_raytracing::math::Vec3 _center,
                                            float                     _half_size);
};

} // namespace cg_raytracing::geometry