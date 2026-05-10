#include <catch2/catch_assertion_result.hpp>
#include <catch2/catch_test_macros.hpp>
#include "vec3.hpp"

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
}

