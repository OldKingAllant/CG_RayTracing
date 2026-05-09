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
}

