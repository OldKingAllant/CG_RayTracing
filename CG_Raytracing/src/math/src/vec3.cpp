#include "vec3.hpp"
using namespace cg_raytracing::math;

Vec3 Vec3::operator+(const Vec3 &_other) const {
    return cg_raytracing::math::Vec3(_other.x + this->x, _other.y + this->y,
                                     _other.z + this->z);
}
