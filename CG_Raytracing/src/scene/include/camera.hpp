#pragma once

#include "config.hpp"
#include "vec3.hpp"
#include <array>
#include <cstdint>
#include <iostream>

namespace cg_raytracing {
namespace scene {

class Camera {
  public:
    float m_sensor_size_width;
    float m_sensor_size_height;
    uint32_t m_focal_length;
    uint32_t m_image_width;
    uint32_t m_image_height;
    std::array<uint8_t, Config::IMAGE_HEIGHT * Config::IMAGE_WIDTH> m_img_buf;

    cg_raytracing::math::Vec3 m_position;
    cg_raytracing::math::Vec3 m_direction;

    // vector that connects the camera focal center to the top left corner.
    cg_raytracing::math::Vec3 m_top_left;
    // vector that indicates the offset to move 1px to the right
    cg_raytracing::math::Vec3 m_horizontal_offset;
    // vector that indicates the offset to move 1px to the bottom
    cg_raytracing::math::Vec3 m_vertical_offset;

    Camera(uint32_t _sensor_size_width = Config::SENSOR_SIZE_WIDTH,
           uint32_t _focal_length = Config::FOCAL_LENGTH,
           uint32_t _image_width = Config::IMAGE_WIDTH,
           uint32_t _image_height = Config::IMAGE_HEIGHT,
           const float *_position = Config::CAMERA_POSITION,
           const float *_direction = Config::CAMERA_DIRECTION);

    void BurstRays();
};
} // namespace scene
} // namespace cg_raytracing
