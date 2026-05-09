#include "ray.hpp"
#include "vec3.hpp"

using namespace cg_raytracing::math;

Ray::Ray(Vec3 _origin, Vec3 _direction, Vec3 _color) {
    this->m_color = _color;
    this->m_direction = _direction;
    this->m_origin = _origin;
}

void Ray::Simulate() {
    this->m_color = Vec3(0.0, 0.0, 0.0);
}
