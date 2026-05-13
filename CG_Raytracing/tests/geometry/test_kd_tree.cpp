#include <kd_tree.hpp>
#include <cube.hpp>
#include <material.hpp>
#include <vec3.hpp>

#include <vector>
#include <memory>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_assertion_result.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("KD Tree tests") {
	SECTION("Single object intersection") {
		using Cube = cg_raytracing::geometry::Cube;
		using Hittable = cg_raytracing::geometry::Hittable;
		using Vec3 = cg_raytracing::math::Vec3;
		using Ray = cg_raytracing::math::Ray;

		std::vector<std::shared_ptr<Hittable>> objects{};
		auto material = cg_raytracing::geometry::Material::Diffuse({});
		objects.push_back(std::make_shared<Cube>(Vec3(.0f, .0f, .0f), 10.f, material));

		auto kd_tree = cg_raytracing::geometry::KDTree::CreateFromHittables(objects, 50.f);

		// Test ray INSIDE the object
		auto intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(0.f, .0f, .0f), Vec3(1.f, .0f, .0f)));
		REQUIRE(intersected.size() == 1);
		// Test negative x towards positive x
		intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(-10.f, .0f, .0f), Vec3(1.f, .0f, .0f)));
		REQUIRE(intersected.size() == 1);
		// Test positive x towards positive x
		intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(20.f, .0f, .0f), Vec3(1.f, .0f, .0f)));
		REQUIRE(intersected.size() == 0);
		// Test neg y towards positive y
		intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(0.f, -20.0f, .0f), Vec3(0.f, 1.0f, .0f)));
		REQUIRE(intersected.size() == 1);
		// Test pos y towards positive y
		intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(0.f, 20.0f, .0f), Vec3(0.f, 1.0f, .0f)));
		REQUIRE(intersected.size() == 0);
		// Test pos y towards neg y
		intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(0.f, 20.0f, .0f), Vec3(0.f, -1.0f, .0f)));
		REQUIRE(intersected.size() == 1);
		// Test neg xyz towards positive xyz
		intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(-20.f, -20.0f, -20.0f), Vec3(1.f, 1.0f, 1.0f)));
		REQUIRE(intersected.size() == 1);
		// Test pos xyz towards positive xyz
		intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(20.f, 20.0f, 20.0f), Vec3(1.f, 1.0f, 1.0f)));
		REQUIRE(intersected.size() == 0);
		// Test pos xyz towards neg xyz
		intersected = kd_tree.RayIntersectsObjects(Ray(Vec3(20.f, 20.0f, 20.0f), Vec3(-1.f, -1.0f, -1.0f)));
		REQUIRE(intersected.size() == 1);
	}
}