#include "camera.hpp"
#include "cube.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "vec3.hpp"
#include "point_light.hpp"
#include <algorithm>


using namespace cg_raytracing::scene;

Camera::Camera(uint32_t _sensor_size_width, uint32_t _focal_length,
               uint32_t _image_width, uint32_t _image_height,
               const float *_position, const float *_direction)
    : m_sensor_size_width(_sensor_size_width),
      m_sensor_size_height(_sensor_size_width / Config::ASPECT_RATIO),
      m_focal_length(_focal_length), m_image_width(_image_width),
      m_image_height(_image_height),
      m_position(_position[0], _position[1], _position[2]),
      m_direction(_direction[0], _direction[1], _direction[2]) {

    // get the top left corner from which calculate all the other rays direction
    math::Vec3 top_left =
        math::Vec3(-1 * this->m_sensor_size_width / 2,
                   -1 * this->m_sensor_size_height / 2, _focal_length);
    top_left += this->m_position;

    // get the offset for each ray for each pixel
    math::Vec3 horizontal_offset =
        math::Vec3(this->m_sensor_size_width / m_image_width, 0, 0);
    math::Vec3 vertical_offset =
        math::Vec3(0, this->m_sensor_size_height / m_image_height, 0);

    // calculate the direction of the rays for each pixel
    for (uint32_t y = 0; y < this->m_image_height; y++) {
        for (uint32_t x = 0; x < this->m_image_width; x++) {
            math::Vec3 ray_direction =
                top_left + horizontal_offset * x + vertical_offset * y;
            ray_direction.Rotate(this->m_direction);
            this->m_rays_matrix[y * this->m_image_width + x].SetDirection(
                ray_direction);
            this->m_rays_matrix[y * this->m_image_width + x].SetOrigin(
                this->m_position);
        }
    }
}



void Camera::Rotate(const math::Vec3 &_rotation_angles) {
    this->m_direction.Rotate(_rotation_angles);

    for (auto &Ray : this->m_rays_matrix) {
        Ray.Rotate(_rotation_angles);
    }
}

void Camera::Translate(const math::Vec3 &_translation_vector) {
    this->m_position += _translation_vector;
    for (auto &Ray : this->m_rays_matrix) {
        Ray.Translate(_translation_vector);
    }
}

void Camera::BurstRays(cg_raytracing::scene::PointLight& light,
                        const geometry::KDTree* kdtree,
                        const std::vector<std::shared_ptr<geometry::Hittable>>* hittables) {

    // Materials
    geometry::Material mat_sphere =
        geometry::Material::Diffuse({0.7f, 0.2f, 0.2f});

    geometry::Material mat_cube =
        geometry::Material::Metal({0.2f, 0.2f, 0.8f}, 0.5f);

    // Scene objects
    geometry::Sphere sphere(
        math::Vec3(-40.0f, 0.0f, 200.0f),
        30.0f,
        mat_sphere
    );

    geometry::Cube cube(
        math::Vec3(40.0f, 0.0f, 200.0f),
        20.0f,
        mat_cube
    );


    int hit_count = 0;

    for (uint32_t y = 0; y < this->m_image_height; y++) {

        for (uint32_t x = 0; x < this->m_image_width; x++) {

            uint32_t base_idx =
                (y * this->m_image_width + x) * 3;

            const math::Ray& ray =
                this->m_rays_matrix[
                    y * this->m_image_width + x
                ];

            // Intersections
            auto hit_sphere = sphere.Hit(ray);
            auto hit_cube   = cube.Hit(ray);

            std::optional<geometry::HitRecord> hit;

            if (hit_sphere && hit_cube) {

                hit =
                    (hit_sphere->m_t < hit_cube->m_t)
                    ? hit_sphere
                    : hit_cube;

            } else if (hit_sphere) {

                hit = hit_sphere;

            } else if (hit_cube) {

                hit = hit_cube;
            }

            if (kdtree && hittables) {
                auto candidates = kdtree->RayIntersectsObjects(ray);
                for (auto& [node, point] : candidates) {
                    if (!node->obj_index.has_value()) continue;
                    auto hit_candidate = (*hittables)[node->obj_index.value()]->Hit(ray);
                    if (hit_candidate) {
                        if (!hit || hit_candidate->m_t < hit->m_t)
                            hit = hit_candidate;
                    }
                }
            }

            // Shading
            if (hit) {

                hit_count++;

                // Direction from surface point to light
                math::Vec3 light_dir =
                    (light.m_position - hit->m_point).normalized();

                // Lambert diffuse shading
                float diffuse =
                    std::max(
                        hit->m_normal.dot(light_dir),
                        0.0f
                    );

                // Final color
                math::Vec3 color =
                    hit->m_material->m_albedo *
                    light.m_color *
                    diffuse *
                    light.m_intensity;

                // Clamp to [0,1]
                color.x = std::clamp(color.x, 0.0f, 1.0f);
                color.y = std::clamp(color.y, 0.0f, 1.0f);
                color.z = std::clamp(color.z, 0.0f, 1.0f);

                // RGB output
                this->m_img_buf[base_idx] =
                    static_cast<uint8_t>(color.x * 255.0f);

                this->m_img_buf[base_idx + 1] =
                    static_cast<uint8_t>(color.y * 255.0f);

                this->m_img_buf[base_idx + 2] =
                    static_cast<uint8_t>(color.z * 255.0f);

            } else {

                // Background gradient
                float t =
                    static_cast<float>(y) /
                    this->m_image_height;

                this->m_img_buf[base_idx] = 0;

                this->m_img_buf[base_idx + 1] =
                    static_cast<uint8_t>(
                        (1.0f - t) * 180 + t * 80
                    );

                this->m_img_buf[base_idx + 2] = 255;
            }
        }
    }

    // Debug stats
    int hit_sphere_count = 0;
    int hit_cube_count   = 0;

    for (uint32_t y = 0; y < this->m_image_height; y++) {

        for (uint32_t x = 0; x < this->m_image_width; x++) {

            const math::Ray& ray =
                this->m_rays_matrix[
                    y * this->m_image_width + x
                ];

            if (sphere.Hit(ray))
                hit_sphere_count++;

            if (cube.Hit(ray))
                hit_cube_count++;
        }
    }

    std::println(std::cout, "Total hits: {}", hit_count);
    std::println(std::cout, "Sphere hits: {}", hit_sphere_count);
    std::println(std::cout, "Cube hits: {}", hit_cube_count);
}

/*
void Camera::BurstRays() {
    // Define materials
    geometry::Material mat_sphere =
        geometry::Material::Diffuse({0.8f, 0.2f, 0.2f});
    geometry::Material mat_cube =
        geometry::Material::Metal({0.9f, 0.1f, 0.1f}, 0.5f);

    geometry::Sphere sphere(math::Vec3(-40.0f, 0.0f, 200.0f), 30.0f,
                            mat_sphere);

    geometry::Cube cube(math::Vec3(40.0f, 0.0f, 200.0f), 20.0f, mat_cube);

    int hit_count = 0;
    for (uint32_t y = 0; y < this->m_image_height; y++) {
        for (uint32_t x = 0; x < this->m_image_width; x++) {
            uint32_t base_idx = (y * this->m_image_width + x) * 3;
            const math::Ray &ray =
                this->m_rays_matrix[y * this->m_image_width + x];

            // Test both objects, keep the closest hit
            auto hit_sphere = sphere.Hit(ray);
            auto hit_cube = cube.Hit(ray);

            std::optional<geometry::HitRecord> hit;
            if (hit_sphere && hit_cube)
                hit = (hit_sphere->m_t < hit_cube->m_t) ? hit_sphere : hit_cube;
            else if (hit_sphere)
                hit = hit_sphere;
            else if (hit_cube)
                hit = hit_cube;

            if (hit) {
                hit_count++;
                this->m_img_buf[base_idx] =
                    (uint8_t)((hit->m_normal.x + 1.0f) * 0.5f * 255);
                this->m_img_buf[base_idx + 1] =
                    (uint8_t)((hit->m_normal.y + 1.0f) * 0.5f * 255);
                this->m_img_buf[base_idx + 2] =
                    (uint8_t)((hit->m_normal.z + 1.0f) * 0.5f * 255);
            } else {
                float t = (float)y / this->m_image_height;
                this->m_img_buf[base_idx] = 0;
                this->m_img_buf[base_idx + 1] =
                    (uint8_t)((1.0f - t) * 180 + t * 80);
                this->m_img_buf[base_idx + 2] = 255;
            }
        }
    }

    // Debug prints
    int hit_sphere_count = 0, hit_cube_count = 0;
    for (uint32_t y = 0; y < this->m_image_height; y++) {
        for (uint32_t x = 0; x < this->m_image_width; x++) {
            const math::Ray &ray =
                this->m_rays_matrix[y * this->m_image_width + x];
            if (sphere.Hit(ray))
                hit_sphere_count++;
            if (cube.Hit(ray))
                hit_cube_count++;
        }
    }
    std::println(std::cout, "Total hits: {}", hit_count);
    std::println(std::cout, "Sphere hits: {}", hit_sphere_count);
    std::println(std::cout, "Cube hits: {}", hit_cube_count);
}

*/