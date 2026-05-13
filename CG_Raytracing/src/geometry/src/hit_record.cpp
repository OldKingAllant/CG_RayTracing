#include "hit_record.hpp"

namespace cg_raytracing::geometry {

void HitRecord::SetFaceNormal(const cg_raytracing::math::Ray&  _ray,
                               const cg_raytracing::math::Vec3& _outward_normal) {
    m_front_face = _ray.m_direction.dot(_outward_normal) < 0.0f;
    m_normal     = m_front_face ? _outward_normal : _outward_normal * -1.0f;
}

} // namespace cg_raytracing::geometry