#pragma once

#include <cstdint>
#include <optional>

#include <vec3.hpp>
#include <ray.hpp>
#include <hit_record.hpp>

namespace cg_raytracing::geometry {
	struct BoundingBox {
		math::Vec3 pos{};
		//float size_x, size_y, size_z;
		float min_x{}, max_x{};
		float min_y{}, max_y{};
		float min_z{}, max_z{};

		static BoundingBox Create(float _x, float _y, float _z,
			float _size_x, float _size_y, float _size_z) noexcept;

		static BoundingBox Create(math::Vec3 const& _center,
			float _size_x, float _size_y, float _size_z) noexcept;

		std::optional<math::Vec3> BBIntersect(BoundingBox const& _other) const noexcept;

		BoundingBox Union(BoundingBox const& _other) const noexcept;

		float SizeX() const noexcept;
		float SizeY() const noexcept;
		float SizeZ() const noexcept;

		float Volume() const noexcept;

		std::optional<math::Vec3> RayIntersect(math::Ray const& _ray, 
			float _t_min = 0.001f,
			float _t_max = 1e9) const;

		std::optional<math::Vec3> SphereIntersect(math::Vec3 const& _p, float _r) const;
	};

	class Hittable {
	public :
		static constexpr float TMIN = 0.001f;
		static constexpr float TMAX = 1e9;

		virtual BoundingBox GetBoundingBox() const = 0;

		virtual std::optional<HitRecord> Hit(const math::Ray& _ray,
			float _t_min = TMIN,
			float _t_max = TMAX) const = 0;
	};
}