#include "camera.hpp"
#include "vec3.hpp"

using namespace cg_raytracing::scene;

Camera::Camera(uint32_t _sensor_size_width, uint32_t _focal_length,
               uint32_t _image_width, uint32_t _image_height,
               const float *_position, const float *_direction)
    : m_sensor_size_width(_sensor_size_width),
      m_sensor_size_height(Config::IMAGE_WIDTH / Config::ASPECT_RATIO),
      m_focal_length(_focal_length), m_image_width(_image_width),
      m_image_height(_image_height),
      m_position(_position[0], _position[1], _position[2]),
      m_direction(_direction[0], _direction[1], _direction[2]) {

    m_top_left = math::Vec3(-1 * this->m_sensor_size_width / 2,
                            -1 * this->m_sensor_size_height, _focal_length);

    // the offest will be the sensor size in mm divided by the amount of pixels
    m_horizontal_offset =
        math::Vec3(this->m_sensor_size_width / m_image_width, 0, 0);
    m_vertical_offset =
        math::Vec3(0, this->m_sensor_size_height / m_image_height, 0);
}

void Camera::BurstRays() {}
