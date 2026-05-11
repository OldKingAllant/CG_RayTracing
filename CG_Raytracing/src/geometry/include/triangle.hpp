#pragma once

#include "hit_record.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <optional>
#include <cmath>

namespace cg_raytracing::geometry {

class Triangle {
public:
    cg_raytracing::math::Vec3 m_v0; // first vertex
    cg_raytracing::math::Vec3 m_v1; // second vertex
    cg_raytracing::math::Vec3 m_v2; // third vertex
    const Material*           m_material;

    Triangle(cg_raytracing::math::Vec3 _v0,
             cg_raytracing::math::Vec3 _v1,
             cg_raytracing::math::Vec3 _v2,
             const Material*           _material)
        : m_v0(_v0), m_v1(_v1), m_v2(_v2), m_material(_material) {}

    // Möller-Trumbore ray-triangle intersection.
    // Computes barycentric coordinates (u, v) and ray parameter t.
    // Returns nullopt if the ray misses or is parallel to the triangle.
    std::optional<HitRecord> Hit(const cg_raytracing::math::Ray& _ray,
                                 float _t_min = 0.001f,
                                 float _t_max = 1e9f) const {
        const auto edge1 = m_v1 - m_v0;
        const auto edge2 = m_v2 - m_v0;

        const auto h = edge2.CrossProduct(_ray.m_direction);
        const float a = edge1.dot(h);

        // Ray is parallel to triangle
        if (std::abs(a) < 1e-8f)
            return std::nullopt;

        const float inv_a = 1.0f / a;
        const auto  s     = _ray.m_origin - m_v0;
        const float u     = inv_a * s.dot(h);

        // Outside triangle
        if (u < 0.0f || u > 1.0f)
            return std::nullopt;

        const auto  q = edge1.CrossProduct(s);
        const float v = inv_a * _ray.m_direction.dot(q);

        // Outside triangle
        if (v < 0.0f || u + v > 1.0f)
            return std::nullopt;

        const float t = inv_a * edge2.dot(q);

        if (t < _t_min || t > _t_max)
            return std::nullopt;

        // Outward normal = normalized cross product of the two edges
        const auto outward_normal = edge1.CrossProduct(edge2).normalized();

        HitRecord rec{};
        rec.m_t        = t;
        rec.m_point    = _ray.At(t);
        rec.m_material = m_material;
        rec.SetFaceNormal(_ray, outward_normal);

        return rec;
    }
};

} // namespace cg_raytracing::geometry