#pragma once

#include "config.hpp"
#include "vec3.hpp"
#include <cstdint>
#include <iostream>

class Camera {
  public:
    Camera(uint32_t _sensor_size_width = Config::SENSOR_SIZE_WIDTH,
           uint32_t _focal_length = Config::FOCAL_LENGTH,
           uint32_t _width = Config::IMAGE_WIDTH,
           uint32_t _height = Config::IMAGE_HEIGHT,
           const float *_position = Config::CAMERA_POSITION,
           const float *_direction = Config::CAMERA_DIRECTION)
        : m_sensor_size_width(_sensor_size_width),
          m_focal_length(_focal_length), m_width(_width), m_height(_height),
          m_position(_position[0], _position[1], _position[2]),
          m_direction(_direction[0], _direction[1], _direction[2]) {}

    uint32_t m_sensor_size_width;
    uint32_t m_focal_length;
    uint32_t m_width;
    uint32_t m_height;
    cg_raytracing::math::Vec3 m_position;
    cg_raytracing::math::Vec3 m_direction;
};
