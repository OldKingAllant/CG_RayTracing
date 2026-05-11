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
    Material m_material;

    // 8 vertices of the cube
    // Built from center + half_size
    std::array<Triangle, 12> m_triangles;

    Cube(cg_raytracing::math::Vec3 _center, float _half_size, Material _material)
        : m_material(_material),
          m_triangles(BuildTriangles(_center, _half_size)) {}

    std::optional<HitRecord> Hit(const cg_raytracing::math::Ray& _ray,
                                 float _t_min = 0.001f,
                                 float _t_max = 1e9f) const {
        std::optional<HitRecord> closest;
        float closest_t = _t_max;

        // Test all 12 triangles, keep the closest hit
        for (const auto& tri : m_triangles) {
            auto hit = tri.Hit(_ray, _t_min, closest_t);
            if (hit) {
                closest_t = hit->m_t;
                closest   = hit;
            }
        }

        return closest;
    }

private:
    std::array<Triangle, 12> BuildTriangles(cg_raytracing::math::Vec3 _center,
                                             float                     _half_size) {
        const float s = _half_size;
        const float cx = _center.x, cy = _center.y, cz = _center.z;

        // 8 corners of the cube
        const cg_raytracing::math::Vec3 v000(cx-s, cy-s, cz-s);
        const cg_raytracing::math::Vec3 v100(cx+s, cy-s, cz-s);
        const cg_raytracing::math::Vec3 v010(cx-s, cy+s, cz-s);
        const cg_raytracing::math::Vec3 v110(cx+s, cy+s, cz-s);
        const cg_raytracing::math::Vec3 v001(cx-s, cy-s, cz+s);
        const cg_raytracing::math::Vec3 v101(cx+s, cy-s, cz+s);
        const cg_raytracing::math::Vec3 v011(cx-s, cy+s, cz+s);
        const cg_raytracing::math::Vec3 v111(cx+s, cy+s, cz+s);

        const Material* mat = &m_material;

        // 6 faces × 2 triangles each = 12 triangles
        return {{
            // Front face  (z+)
            { v001, v101, v111, mat },
            { v001, v111, v011, mat },
            // Back face   (z-)
            { v100, v000, v010, mat },
            { v100, v010, v110, mat },
            // Left face   (x-)
            { v000, v001, v011, mat },
            { v000, v011, v010, mat },
            // Right face  (x+)
            { v101, v100, v110, mat },
            { v101, v110, v111, mat },
            // Top face    (y+)
            { v010, v011, v111, mat },
            { v010, v111, v110, mat },
            // Bottom face (y-)
            { v000, v100, v101, mat },
            { v000, v101, v001, mat },
        }};
    }
};

} // namespace cg_raytracing::geometry


/*

#pragma once

#include "hit_record.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <optional>
#include <cmath>
#include <algorithm>

namespace cg_raytracing::geometry {

class Cube {
public:
    cg_raytracing::math::Vec3 m_min;
    cg_raytracing::math::Vec3 m_max;
    Material                  m_material;

    // Build a cube from center + half-extent
    Cube(cg_raytracing::math::Vec3 _center, float _half_size, Material _material)
        : m_min(_center.x - _half_size,
                _center.y - _half_size,
                _center.z - _half_size),
          m_max(_center.x + _half_size,
                _center.y + _half_size,
                _center.z + _half_size),
          m_material(_material) {}

    // Ray-slab intersection
    std::optional<HitRecord> Hit(const cg_raytracing::math::Ray& _ray,
                                 float _t_min = 0.001f,
                                 float _t_max = 1e9f) const {

        float t_enter = _t_min;
        float t_exit  = _t_max;

        int hit_axis  = -1;
        bool hit_negative = false;

        const float origins[3]    = { _ray.m_origin.x,    _ray.m_origin.y,    _ray.m_origin.z };
        const float directions[3] = { _ray.m_direction.x, _ray.m_direction.y, _ray.m_direction.z };
        const float mins[3]       = { m_min.x, m_min.y, m_min.z };
        const float maxs[3]       = { m_max.x, m_max.y, m_max.z };

        for (int i = 0; i < 3; i++) {
            if (std::abs(directions[i]) < 1e-8f) {
                // Ray is parallel to this slab — check if origin is inside
                if (origins[i] < mins[i] || origins[i] > maxs[i])
                    return std::nullopt;
                continue;
            }

            const float inv_d = 1.0f / directions[i];
            float t0 = (mins[i] - origins[i]) * inv_d; // hit near plane
            float t1 = (maxs[i] - origins[i]) * inv_d; // hit far plane

            bool negative_dir = inv_d < 0.0f;
            if (negative_dir) std::swap(t0, t1);

            if (t0 > t_enter) {
                t_enter    = t0;
                hit_axis   = i;
                hit_negative = negative_dir;
            }
            t_exit = std::min(t_exit, t1);

            if (t_exit <= t_enter)
                return std::nullopt;
        }

        if (t_enter < _t_min || t_enter > _t_max)
            return std::nullopt;

        // Build the outward normal for the face the ray entered through
        cg_raytracing::math::Vec3 outward_normal(0.0f, 0.0f, 0.0f);
        if      (hit_axis == 0) outward_normal.x = hit_negative ? 1.0f : -1.0f;
        else if (hit_axis == 1) outward_normal.y = hit_negative ? 1.0f : -1.0f;
        else                    outward_normal.z = hit_negative ? 1.0f : -1.0f;

        HitRecord rec{};
        rec.m_t        = t_enter;
        rec.m_point    = _ray.At(t_enter);
        rec.m_material = &m_material;
        rec.SetFaceNormal(_ray, outward_normal);

        return rec;
    }
};

} // namespace cg_raytracing::geometry

*/