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

Vec3 &Vec3::operator=(const Vec3 &_other) {
    this->x = _other.x;
    this->y = _other.y;
    this->z = _other.z;

    return *this;
}

double Vec3::DotProduct(const Vec3 &_other) const {
    return this->x * _other.x + this->y * _other.y + this->z * _other.z;
}
Vec3 Vec3::CrossProduct(const Vec3 &_other) const {
    return Vec3(this->y * _other.z - this->z * _other.y,
                this->z * _other.x - this->x * _other.z,
                this->x * _other.y - this->y * _other.x);
}

Vec3 Vec3::operator-(const Vec3 &_other) const {
    return Vec3(this->x - _other.x, this->y - _other.y, this->z - _other.z);
}

float Vec3::dot(const Vec3 &_other) const {
    return this->x * _other.x + this->y * _other.y + this->z * _other.z;
}

float Vec3::length_squared() const {
    return this->x * this->x + this->y * this->y + this->z * this->z;
}

float Vec3::length() const {
    return std::sqrt(length_squared());
}

Vec3 Vec3::normalized() const {
    float len = length();
    if (len == 0.0f) return Vec3(0, 0, 0);
    return *this / len;
}