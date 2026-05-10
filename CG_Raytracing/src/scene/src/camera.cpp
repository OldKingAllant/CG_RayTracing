#include "camera.hpp"
#include "ray.hpp"
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

    math::Vec3 top_left =
        math::Vec3(-1 * this->m_sensor_size_width / 2,
                   -1 * this->m_sensor_size_height, _focal_length);

    // the offest will be the sensor size in mm divided by the amount of pixels
    math::Vec3 horizontal_offset =
        math::Vec3(this->m_sensor_size_width / m_image_width, 0, 0);
    math::Vec3 vertical_offset =
        math::Vec3(0, this->m_sensor_size_height / m_image_height, 0);

    // create the ray matrix
    for (uint32_t x = 0; x < this->m_image_width; x++) {
        for (uint32_t y = 0; y < this->m_image_height; y++) {
            math::Vec3 ray_direction =
                top_left + horizontal_offset * x + vertical_offset * y;
            this->m_rays_matrix[x + y * this->m_image_width].SetDirection(
                ray_direction);

            this->m_rays_matrix[x + y * this->m_image_width].SetOrigin(
                this->m_position);

        }
    }
}

void Camera::BurstRays() {
    for (uint32_t x = 0; x < this->m_image_width; x++) {
        for (uint32_t y = 0; y < this->m_image_height; y++) {
            math::Ray ray = math::Ray(m_position, m_direction);
            ray.Simulate();
            uint32_t base_idx = (y * this->m_image_width + x) * 3;

            this->m_img_buf[base_idx] =
                (uint8_t)(x * 255 / this->m_image_width);
            this->m_img_buf[base_idx + 1] =
                (uint8_t)(y * 255 / this->m_image_height);
            this->m_img_buf[base_idx + 2] = 255;
        }
    }
}
