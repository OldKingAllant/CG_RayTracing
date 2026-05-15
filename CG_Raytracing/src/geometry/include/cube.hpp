#pragma once

#include "hit_record.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "triangle.hpp"
#include "vec3.hpp"
#include <hittable.hpp>

#include <optional>
#include <array>

namespace cg_raytracing::geometry {

    class Cube : public Hittable {
    public:
        Material                 m_material;
        std::array<Triangle, 12> m_triangles;
        math::Vec3               m_center;
        float                    m_size;
    
        Cube(math::Vec3 _center, float _half_size, Material _material);
    
        std::optional<HitRecord> Hit(const math::Ray& _ray,
            float _t_min = TMIN,
            float _t_max = TMAX) const override;

        BoundingBox GetBoundingBox() const override;
    
    private:
        std::array<Triangle, 12> BuildTriangles(math::Vec3 _center,
                                                float      _half_size);
    };

} // namespace cg_raytracing::geometry