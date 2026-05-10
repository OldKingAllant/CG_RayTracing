#include "camera.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "sphere.hpp"
#include "material.hpp"

using namespace cg_raytracing::scene;

// Il costruttore rimane invariato
Camera::Camera(uint32_t _sensor_size_width, uint32_t _focal_length,
               uint32_t _image_width, uint32_t _image_height,
               const float *_position, const float *_direction)
    : m_sensor_size_width(_sensor_size_width),
      m_sensor_size_height(_sensor_size_width / Config::ASPECT_RATIO),
      m_focal_length(_focal_length), m_image_width(_image_width),
      m_image_height(_image_height),
      m_position(_position[0], _position[1], _position[2]),
      m_direction(_direction[0], _direction[1], _direction[2]) {

    math::Vec3 top_left = math::Vec3(
        -1 * this->m_sensor_size_width / 2,
        -1 * this->m_sensor_size_height / 2,  // ← dividi per 2
        _focal_length
    );

    math::Vec3 horizontal_offset =
        math::Vec3(this->m_sensor_size_width / m_image_width, 0, 0);
    math::Vec3 vertical_offset =
        math::Vec3(0, this->m_sensor_size_height / m_image_height, 0);

    for (uint32_t y = 0; y < this->m_image_height; y++) {
        for (uint32_t x = 0; x < this->m_image_width; x++) {
            math::Vec3 ray_direction =
                top_left + horizontal_offset * x + vertical_offset * y;
            this->m_rays_matrix[y * this->m_image_width + x].SetDirection(ray_direction);
            this->m_rays_matrix[y * this->m_image_width + x].SetOrigin(this->m_position);
        }
}
}

/*void Camera::BurstRays() {
    // Definisci la scena: una sfera rossa al centro davanti alla camera
    geometry::Sphere sphere(
        math::Vec3(Config::FOCAL_LENGTH, 0.0f, 0.0f),  // posizione: lungo X
        10.0f,                                           // raggio in mm
        geometry::Material::Diffuse({0.8f, 0.2f, 0.2f})
    );

    for (uint32_t x = 0; x < this->m_image_width; x++) {
        for (uint32_t y = 0; y < this->m_image_height; y++) {
            uint32_t base_idx = (y * this->m_image_width + x) * 3;

            const math::Ray &ray = this->m_rays_matrix[x + y * this->m_image_width];
            auto hit = sphere.hit(ray);

            if (hit) {
                // Colora in base alla normale: mappa [-1,1] → [0,255]
                this->m_img_buf[base_idx]     = (uint8_t)((hit->normal.x + 1.0f) * 0.5f * 255);
                this->m_img_buf[base_idx + 1] = (uint8_t)((hit->normal.y + 1.0f) * 0.5f * 255);
                this->m_img_buf[base_idx + 2] = (uint8_t)((hit->normal.z + 1.0f) * 0.5f * 255);
            } else {
                // Sfondo: gradiente azzurro
                float t = (float)y / this->m_image_height;
                this->m_img_buf[base_idx]     = 0;
                this->m_img_buf[base_idx + 1] = (uint8_t)((1.0f - t) * 180 + t * 80);
                this->m_img_buf[base_idx + 2] = 255;
            }
        }
    }
}*/

void Camera::BurstRays() {
    geometry::Sphere sphere(
    math::Vec3(0.0f, 0.0f, 200.0f),
    30.0f,
    geometry::Material::Diffuse({0.8f, 0.2f, 0.2f})
);

    int hit_count = 0;  // ← conta i raggi che colpiscono

    for (uint32_t y = 0; y < this->m_image_height; y++) {
        for (uint32_t x = 0; x < this->m_image_width; x++) {
            uint32_t base_idx = (y * this->m_image_width + x) * 3;
            const math::Ray &ray = this->m_rays_matrix[y * this->m_image_width + x];
            
            auto hit = sphere.hit(ray);
        

            if (hit) {
                hit_count++;
                this->m_img_buf[base_idx]     = (uint8_t)((hit->normal.x + 1.0f) * 0.5f * 255);
                this->m_img_buf[base_idx + 1] = (uint8_t)((hit->normal.y + 1.0f) * 0.5f * 255);
                this->m_img_buf[base_idx + 2] = (uint8_t)((hit->normal.z + 1.0f) * 0.5f * 255);
            } else {
                float t = (float)y / this->m_image_height;
                this->m_img_buf[base_idx]     = 0;
                this->m_img_buf[base_idx + 1] = (uint8_t)((1.0f - t) * 180 + t * 80);
                this->m_img_buf[base_idx + 2] = 255;
            }
        }
    }

    std::println(std::cout, "Hit count: {}", hit_count);
std::println(std::cout, "Ray[center] direction: ({}, {}, {})",
    m_rays_matrix[(m_image_height/2) * m_image_width + m_image_width/2].m_direction.x,
    m_rays_matrix[(m_image_height/2) * m_image_width + m_image_width/2].m_direction.y,
    m_rays_matrix[(m_image_height/2) * m_image_width + m_image_width/2].m_direction.z);
std::println(std::cout, "Ray[0] direction: ({}, {}, {})",
    m_rays_matrix[0].m_direction.x,
    m_rays_matrix[0].m_direction.y,
    m_rays_matrix[0].m_direction.z);
std::println(std::cout, "Sensor width: {} height: {}", m_sensor_size_width, m_sensor_size_height);
    std::println(std::cout, "Hit count: {}", hit_count);  // ← stampa quanti hit
    
    // Stampa il primo raggio per capire la direzione
    std::println(std::cout, "Ray[0] origin:    ({}, {}, {})",
        m_rays_matrix[0].m_origin.x,
        m_rays_matrix[0].m_origin.y,
        m_rays_matrix[0].m_origin.z);
    std::println(std::cout, "Ray[0] direction: ({}, {}, {})",
        m_rays_matrix[0].m_direction.x,
        m_rays_matrix[0].m_direction.y,
        m_rays_matrix[0].m_direction.z);
}