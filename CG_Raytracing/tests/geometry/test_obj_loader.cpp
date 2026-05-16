#include "mesh.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <filesystem>


TEST_CASE("Mesh OBJ Loader") {
    using namespace cg_raytracing::geometry;
    using namespace cg_raytracing::math;
    
    std::filesystem::path test_file_path = "./test_obj_file.obj";


    SECTION("File Not Found Boundary") {
        Material dummy_material;
        Mesh mesh(Vec3(0,0,0), "./invalid.obj", dummy_material);
        auto result = mesh.LoadFromObj("./invalid.obj");
        
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Valid Single Face Geometry Integrity") {

        Material dummy_material;
        Mesh mesh(Vec3(0,0,0), test_file_path, dummy_material);
        auto result = mesh.LoadFromObj(test_file_path);

        // Verify function execution
        REQUIRE(result.has_value());

        // 1. Validate progressive Vertex assembly
        REQUIRE(mesh.m_vertex_positions.size() == 3);
        REQUIRE(mesh.m_vertex_positions[0].x == Catch::Approx(1.5));
        REQUIRE(mesh.m_vertex_positions[0].y == Catch::Approx(2.5));
        REQUIRE(mesh.m_vertex_positions[0].z == Catch::Approx(3.5));

        // 2. Validate progressive Normal assembly
        REQUIRE(mesh.m_vertex_normals.size() == 1);
        REQUIRE(mesh.m_vertex_normals[0].x == Catch::Approx(0.0));
        REQUIRE(mesh.m_vertex_normals[0].y == Catch::Approx(1.0));
        REQUIRE(mesh.m_vertex_normals[0].z == Catch::Approx(0.0));

        // 3. Validate progressive UV array assembly
        REQUIRE(mesh.m_face_uv.size() == 1);
        REQUIRE(mesh.m_face_uv[0][0] == Catch::Approx(0.25f));
        REQUIRE(mesh.m_face_uv[0][1] == Catch::Approx(0.75f));

        // 4. Validate Smooth Shading State Change
        REQUIRE(mesh.m_smooth_shading == true);

        std::filesystem::remove(test_file_path);
    }
}
