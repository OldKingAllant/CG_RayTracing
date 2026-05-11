#pragma once

#include "hit_record.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <optional>
#include <cmath>

namespace cg_raytracing::geometry {

class Sphere {
public:
    cg_raytracing::math::Vec3 m_center;
    float                     m_radius;
    Material                  m_material;

    Sphere(cg_raytracing::math::Vec3 _center, float _radius, Material _material)
        : m_center(_center), m_radius(_radius), m_material(_material) {}

    // Returns a HitRecord if the ray intersects the sphere within [_t_min, _t_max].
    // Uses the half-vector form of the quadratic to avoid precision loss.
    std::optional<HitRecord> Hit(const cg_raytracing::math::Ray& _ray,
                                 float _t_min = 0.001f,
                                 float _t_max = 1e9f) const {
        const auto oc = _ray.m_origin - m_center;

        const float a  = _ray.m_direction.dot(_ray.m_direction);
        const float hb = oc.dot(_ray.m_direction);
        const float c  = oc.dot(oc) - m_radius * m_radius;

        const float discriminant = hb * hb - a * c;
        if (discriminant < 0.0f)
            return std::nullopt;

        const float sqrtd = std::sqrt(discriminant);

        float root = (-hb - sqrtd) / a;
        if (root <= _t_min || root >= _t_max) {
            root = (-hb + sqrtd) / a;
            if (root <= _t_min || root >= _t_max)
                return std::nullopt;
        }

        HitRecord rec{};
        rec.m_t        = root;
        rec.m_point    = _ray.At(root);
        rec.m_material = &m_material;
        rec.SetFaceNormal(_ray, (rec.m_point - m_center) * (1.0f / m_radius));

        return rec;
    }
};

} // namespace cg_raytracing::geometry