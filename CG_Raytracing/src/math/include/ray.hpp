#pragma once

#include "vec3.hpp"

namespace cg_raytracing {
namespace math {
class Ray {
  public:
    Vec3 m_origin;
    Vec3 m_direction;
    Vec3 m_color;

    Ray(Vec3 _origin = Vec3(0, 0, 0), Vec3 _direction = Vec3(0, 0, 0));

    void SetOrigin(Vec3 &_other);
    void SetDirection(Vec3 &_other);
    void Simulate();
};
} // namespace math
} // namespace cg_raytracing
