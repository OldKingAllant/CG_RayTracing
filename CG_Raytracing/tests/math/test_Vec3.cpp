#include "vec3.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_assertion_result.hpp>
#include <catch2/catch_test_macros.hpp>

#define M_PI           3.14159265358979323846

TEST_CASE("Vec3 basic operations") {
    SECTION("Sum") {
        using namespace cg_raytracing::math;

        Vec3 u(1.0, 1.0, 1.0);
        Vec3 v(1.0, 1.0, 1.0);
        Vec3 result = u + v;

        REQUIRE(result.x == 2.0);
        REQUIRE(result.y == 2.0);
        REQUIRE(result.z == 2.0);
    }

    SECTION("Sum reference") {
        using namespace cg_raytracing::math;

        Vec3 u(1.0, 1.0, 1.0);
        Vec3 v(1.0, 1.0, 1.0);
        u += v;

        REQUIRE(u.x == 2.0);
        REQUIRE(u.y == 2.0);
        REQUIRE(u.z == 2.0);
    }

    SECTION("Multiplication") {
        using namespace cg_raytracing::math;

        Vec3 u(1.0, 1.0, 1.0);
        Vec3 result = u * 2.0;

        REQUIRE(result.x == 2.0);
        REQUIRE(result.y == 2.0);
        REQUIRE(result.z == 2.0);
    }

    SECTION("Multiplication reference") {
        using namespace cg_raytracing::math;

        Vec3 u(1.0, 1.0, 1.0);
        u *= 2.0;

        REQUIRE(u.x == 2.0);
        REQUIRE(u.y == 2.0);
        REQUIRE(u.z == 2.0);
    }
    SECTION("Multiplication") {
        using namespace cg_raytracing::math;

        Vec3 u(2.0, 2.0, 2.0);
        Vec3 result = u / 2.0;

        REQUIRE(result.x == 1.0);
        REQUIRE(result.y == 1.0);
        REQUIRE(result.z == 1.0);
    }

    SECTION("Multiplication reference") {
        using namespace cg_raytracing::math;

        Vec3 u(2.0, 2.0, 2.0);
        u /= 2.0;

        REQUIRE(u.x == 1.0);
        REQUIRE(u.y == 1.0);
        REQUIRE(u.z == 1.0);
    }

    SECTION("Dot product") {
        using namespace cg_raytracing::math;

        Vec3 u(1.0, 2.0, 3.0);
        Vec3 v(4.0, 5.0, 6.0);
        double result = u.DotProduct(v);

        REQUIRE(result == 32.0);
    }

    SECTION("Cross product") {
        using namespace cg_raytracing::math;

        Vec3 u(1.0, 2.0, 3.0);
        Vec3 v(1.0, 5.0, 7.0);
        Vec3 result = u.CrossProduct(v);

        REQUIRE(result.x == -1.0);
        REQUIRE(result.y == -4.0);
        REQUIRE(result.z == 3.0);
    }

    SECTION("Rotation") {
        using namespace cg_raytracing::math;

        Vec3 u(1.0, 0.0, 0.0);
        Vec3 rotation_vector(0.0, 90 * (M_PI / 180),
                             0.0); // Rotate 90 degrees around Y-axis
        u.rotate(rotation_vector);

        REQUIRE(u.x == Catch::Approx(0.0).margin(1e-3));
        REQUIRE(u.y == Catch::Approx(0.0).margin(1e-3));
        REQUIRE(u.z == Catch::Approx(-1.0).margin(1e-3));
    }
}
