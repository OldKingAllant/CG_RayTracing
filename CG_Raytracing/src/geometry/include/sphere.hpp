#pragma once

#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <optional>
#include <cmath>

namespace cg_raytracing::geometry {

struct HitRecord {
    cg_raytracing::math::Vec3 point;
    cg_raytracing::math::Vec3 normal;
    const Material*           material;
    float                     t;
    bool                      front_face;

    void set_face_normal(const cg_raytracing::math::Ray& ray,
                         const cg_raytracing::math::Vec3& outward_normal) {
        front_face = ray.m_direction.dot(outward_normal) < 0.0f;  // ← m_direction
        normal     = front_face ? outward_normal : outward_normal * -1.0f;
    }
};

class Sphere {
public:
    cg_raytracing::math::Vec3 center;
    float                     radius;
    Material                  material;

    Sphere(cg_raytracing::math::Vec3 center, float radius, Material material)
        : center(center), radius(radius), material(material) {}

    std::optional<HitRecord> hit(const cg_raytracing::math::Ray& ray,
                                 float t_min = 0.001f,
                                 float t_max = 1e9f) const {
        const auto oc = ray.m_origin - center;               // ← m_origin

        const float a  = ray.m_direction.dot(ray.m_direction);  // ← m_direction
        const float hb = oc.dot(ray.m_direction);               // ← m_direction
        const float c  = oc.dot(oc) - radius * radius;

        const float discriminant = hb * hb - a * c;
        if (discriminant < 0.0f)
            return std::nullopt;

        const float sqrtd = std::sqrt(discriminant);

        float root = (-hb - sqrtd) / a;
        if (root <= t_min || root >= t_max) {
            root = (-hb + sqrtd) / a;
            if (root <= t_min || root >= t_max)
                return std::nullopt;
        }

        HitRecord rec{};
        rec.t        = root;
        rec.point    = ray.at(root);
        rec.material = &material;
        rec.set_face_normal(ray, (rec.point - center) * (1.0f / radius));

        return rec;
    }
};

} // namespace cg_raytracing::geometry