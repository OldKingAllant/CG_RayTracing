#pragma once

#include "config.hpp"
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
          m_focal_length(_focal_length), m_width(_width), m_height(_height) {
        std::copy(_position, _position + 3, m_position);
        std::copy(_direction, _direction + 3, m_direction);
    }
    uint32_t m_sensor_size_width;
    uint32_t m_focal_length;
    uint32_t m_width;
    uint32_t m_height;
    float m_position[3];
    float m_direction[3];
};
