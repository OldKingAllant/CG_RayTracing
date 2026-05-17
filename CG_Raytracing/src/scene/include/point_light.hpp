#pragma once

#include "vec3.hpp"

namespace cg_raytracing::scene {

struct PointLight {
    cg_raytracing::math::Vec3 m_position;
    cg_raytracing::math::Vec3 m_color;
    float                     m_intensity;

    PointLight(
        cg_raytracing::math::Vec3 _position,
        cg_raytracing::math::Vec3 _color = {1.0f, 1.0f, 1.0f},
        float _intensity = 1.0f)
        : m_position(_position),
          m_color(_color),
          m_intensity(_intensity) {}
};

}