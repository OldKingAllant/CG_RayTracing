#include "ray.hpp"
#include "vec3.hpp"

using namespace cg_raytracing::math;

Ray::Ray(Vec3 _origin, Vec3 _direction) {
    this->m_origin = _origin;
    this->m_direction = _direction;
}

void Ray::SetDirection(Vec3 const &_other) {
    this->m_direction = _other;
}

void Ray::SetOrigin(Vec3 const &_other) {
    this->m_origin = _other;
}

void Ray::Simulate() {
    this->m_color = Vec3(0.0, 0.0, 0.0);
}

void Ray::Rotate(const Vec3 &_rotation_angles) {
    this->m_direction.Rotate(_rotation_angles);
}


void Ray::Translate(const math::Vec3 &_translation_vector) {
    this->m_origin += _translation_vector;
}
