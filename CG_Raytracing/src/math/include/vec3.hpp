#pragma once

namespace cg_raytracing {
namespace math {
class Vec3 {
  public:
    float x, y, z;
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
    Vec3 operator+(const Vec3 &_other) const;
    Vec3 operator+=(const Vec3 &_other);
};
} // namespace math
} // namespace cg_raytracing
