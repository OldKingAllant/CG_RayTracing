#pragma once

namespace cg_raytracing {
namespace math {
class Vec3 {
  public:
    float x, y, z;
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
    Vec3 operator+(const Vec3 &_other) const;
    Vec3& operator+=(const Vec3 &_other);

    // scalar product
    Vec3 operator*(const float _scalar) const;
    Vec3& operator*=(const float _scalar);

    Vec3 operator/(const float _scalar) const;
    Vec3& operator/=(const float _scalar);

    double DotProduct(const Vec3 &_other) const;
    Vec3 CrossProduct(const Vec3 &_other) const;

};
} // namespace math
} // namespace cg_raytracing
