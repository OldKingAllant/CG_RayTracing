#pragma once

#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace cg_raytracing::geometry {

// Stores the result of a ray-surface intersection
struct HitRecord {
    cg_raytracing::math::Vec3 m_point;      // world-space hit position
    cg_raytracing::math::Vec3 m_normal;     // surface normal, always points against the ray
    const Material*           m_material;   // non-owning pointer to the hit surface material
    float                     m_t;          // ray parameter at the hit point
    bool                      m_front_face; // true if the ray hit the outside of the surface

    // Sets the normal so it always faces against the incoming ray direction.
    // This simplifies shading: you never need to check which side was hit.
    void SetFaceNormal(const cg_raytracing::math::Ray&  _ray,
                       const cg_raytracing::math::Vec3& _outward_normal) {
        m_front_face = _ray.m_direction.dot(_outward_normal) < 0.0f;
        m_normal     = m_front_face ? _outward_normal : _outward_normal * -1.0f;
    }
};

} // namespace cg_raytracing::geometry