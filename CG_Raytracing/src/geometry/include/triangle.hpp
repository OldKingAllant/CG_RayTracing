#pragma once

#include "hit_record.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <hittable.hpp>

#include <optional>

namespace cg_raytracing::geometry {

    class Triangle : public Hittable {
    public:
        cg_raytracing::math::Vec3 m_v0;
        cg_raytracing::math::Vec3 m_v1;
        cg_raytracing::math::Vec3 m_v2;
        const Material*           m_material;
    
        Triangle(cg_raytracing::math::Vec3 _v0,
                 cg_raytracing::math::Vec3 _v1,
                 cg_raytracing::math::Vec3 _v2,
                 const Material*           _material);
    
        std::optional<HitRecord> Hit(const cg_raytracing::math::Ray& _ray,
                                     float _t_min = TMIN,
                                     float _t_max = TMAX) const override;

        BoundingBox GetBoundingBox() const override;
    };

} // namespace cg_raytracing::geometry