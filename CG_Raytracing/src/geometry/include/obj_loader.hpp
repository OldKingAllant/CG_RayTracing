#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <iostream>
#include <print>
#include <fstream>
#include <ranges>

namespace cg_raytracing {
namespace geometry {
// FIX: this is just to avoid errors
class Mesh {};

// WARNING:there should be a class mesh that contains the information about the
// object
std::expected<Mesh, std::string> LoadFromObj(std::filesystem::path _obj_path);
}; // namespace geometry
}; // namespace cg_raytracing
