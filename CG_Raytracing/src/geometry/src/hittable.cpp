#include <hittable.hpp>

#include <cmath>
#include <algorithm>

namespace cg_raytracing::geometry {
	BoundingBox BoundingBox::Create(float _x, float _y, float _z,
		float _size_x, float _size_y, float _size_z) noexcept {
		BoundingBox bb{};
		bb.pos = math::Vec3(_x, _y, _z);

		bb.min_x = _x - _size_x / 2;
		bb.min_y = _y - _size_y / 2;
		bb.min_z = _z - _size_z / 2;

		bb.max_x = _x + _size_x / 2;
		bb.max_y = _y + _size_y / 2;
		bb.max_z = _z + _size_z / 2;
		return bb;
	}

	BoundingBox BoundingBox::Create(math::Vec3 const& _center, float _size_x, float _size_y, float _size_z) noexcept {
		BoundingBox bb{};
		bb.pos = _center;

		bb.min_x = _center.x - _size_x / 2;
		bb.min_y = _center.y - _size_y / 2;
		bb.min_z = _center.z - _size_z / 2;

		bb.max_x = _center.x + _size_x / 2;
		bb.max_y = _center.y + _size_y / 2;
		bb.max_z = _center.z + _size_z / 2;
		return bb;
	}

	bool BoundingBox::BBIntersect(BoundingBox const& _other) const noexcept {
		return !(
			min_x > _other.max_x || max_x < _other.min_x ||
			min_y > _other.max_y || max_y < _other.min_y ||
			min_z > _other.max_z || max_z < _other.min_z);
	}

	BoundingBox BoundingBox::Union(BoundingBox const& _other) const noexcept {
		BoundingBox bb{};
		bb.min_x = std::fmin(min_x, _other.min_x);
		bb.min_y = std::fmin(min_y, _other.min_y);
		bb.min_z = std::fmin(min_z, _other.min_z);

		bb.max_x = std::fmax(max_x, _other.max_x);
		bb.max_y = std::fmax(max_y, _other.max_y);
		bb.max_z = std::fmax(max_z, _other.max_z);

		bb.pos = math::Vec3(
			(bb.min_x + bb.max_x) / 2.f,
			(bb.min_y + bb.max_y) / 2.f,
			(bb.min_z + bb.max_z) / 2.f);

		return bb;
	}

	float BoundingBox::SizeX() const noexcept {
		return std::abs(max_x - min_x);
	}

	float BoundingBox::SizeY() const noexcept {
		return std::abs(max_y - min_y);
	}

	float BoundingBox::SizeZ() const noexcept {
		return std::abs(max_z - min_z);
	}

	float BoundingBox::Volume() const noexcept {
		return SizeX() * SizeY() * SizeZ();
	}

	std::optional<math::Vec3> BoundingBox::RayIntersect(math::Ray const& _ray,
		float _t_min,
		float _t_max) const {
		float t_enter = _t_min;
		float t_exit = _t_max;

		int32_t hit_axis = -1;
		bool hit_negative = false;

		const float origins[3] = { _ray.m_origin.x,    _ray.m_origin.y,    _ray.m_origin.z };
		const float directions[3] = { _ray.m_direction.x, _ray.m_direction.y, _ray.m_direction.z };
		const float mins[3] = { min_x, min_y, min_z };
		const float maxs[3] = { max_x, max_y, max_z };

		uint32_t origins_inside_minmax{};

		for (uint32_t i = 0; i < 3; i++) {
			origins_inside_minmax += ((origins[i] >= mins[i]) && (origins[i] <= maxs[i])) ? 1 : 0;

			const float inv_d = 1.0f / directions[i];
			float t0 = (mins[i] - origins[i]) * inv_d; // hit near plane
			float t1 = (maxs[i] - origins[i]) * inv_d; // hit far plane

			bool negative_dir = inv_d < 0.0f;
			if (negative_dir) std::swap(t0, t1);

			if (t0 > t_enter) {
				t_enter = t0;
				hit_axis = i;
				hit_negative = negative_dir;
			}
			t_exit = std::min(t_exit, t1);

			if (t_exit <= t_enter)
				return std::nullopt;
		}

		auto does_hit = (origins_inside_minmax == 3) || (_t_min < t_enter && t_enter < _t_max);

		if (!does_hit) {
			return std::nullopt;
		}

		return _ray.At(origins_inside_minmax != 3 ? t_enter : t_exit);
	}
}