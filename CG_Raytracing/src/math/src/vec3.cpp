#include "vec3.hpp"
#include <stdexcept>
using namespace cg_raytracing::math;

Vec3 Vec3::operator+(const Vec3 &_other) const {
    return Vec3(_other.x + this->x, _other.y + this->y, _other.z + this->z);
}

Vec3 &Vec3::operator+=(const Vec3 &_other) {
    this->x += _other.x;
    this->y += _other.y;
    this->z += _other.z;

    return *this;
}

Vec3 Vec3::operator*(const float _scalar) const {
    return Vec3(this->x * _scalar, this->y * _scalar, this->z * _scalar);
}

Vec3 &Vec3::operator*=(const float _scalar) {
    this->x *= _scalar;
    this->y *= _scalar;
    this->z *= _scalar;

    return *this;
}

Vec3 Vec3::operator/(const float _scalar) const {
    if (_scalar == 0) {
        throw std::runtime_error("Division by zero");
    }
    return Vec3(this->x / _scalar, this->y / _scalar, this->z / _scalar);
};

Vec3 &Vec3::operator/=(const float _scalar) {
    if (_scalar == 0) {
        throw std::runtime_error("Division by zero");
    }
    return *this *= 1 / _scalar;
};

double Vec3::DotProduct(const Vec3 &_other) const {
    return this->x * _other.x + this->y * _other.y + this->z * _other.z;
}
Vec3 Vec3::CrossProduct(const Vec3 &_other) const {
    return Vec3(this->y * _other.z - this->z * _other.y,
                this->z * _other.x - this->x * _other.z,
                this->x * _other.y - this->y * _other.x);
}
