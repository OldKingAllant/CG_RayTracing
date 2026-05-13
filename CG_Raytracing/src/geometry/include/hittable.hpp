#pragma once

#include <cstdint>

#include <vec3.hpp>
#include <ray.hpp>

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

		bool BBIntersect(BoundingBox const& _other) const noexcept;

		BoundingBox Union(BoundingBox const& _other) const noexcept;

		float SizeX() const noexcept;
		float SizeY() const noexcept;
		float SizeZ() const noexcept;

		float Volume() const noexcept;

		bool RayIntersect(math::Ray const& _ray, 
			float _t_min = 0.001f,
			float _t_max = 1e9) const;
	};

	class Hittable {
	public :
		virtual BoundingBox GetBoundingBox() const = 0;
	};
}